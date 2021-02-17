#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct node {   // Dosyadan cekilmis verilerin tutuldugu struct
	char userName[50];      // Kullanici adi : U1...NU5
	char books[50][50];		// Kitaplar
	int givenPoint[8];      // Kitaplara verilen puanlar
	struct node *next;	
}bookRecommendation;

bookRecommendation *front = NULL;
bookRecommendation *rear = NULL;

typedef struct nodeTwo{   // Hesaplanan similarity degerlerinin tutuldugu struct yapisi
	char userName[50];  	// Kullanici adi : NU1...NU5
	char who[50][50];		// Kullanici adi : U1...U20
	float similarity[20];   // Her NU1...NU5 kullanicilari icin U1....U20 arasindaki kullanicilar ile arasindaki similarity degerleri
	struct nodeTwo *next;
}calculatedSimilarities;

calculatedSimilarities *frontTwo = NULL;
calculatedSimilarities *rearTwo = NULL;

bookRecommendation *createNode(){     // Her kullanici ve onlarin okudugu kitaplara verdigi puanlar icin bellekte yer aciyorum.
	bookRecommendation *newInformation = (bookRecommendation*)calloc(sizeof(bookRecommendation),1);
	strcpy(newInformation->books[0],"TRUE BELIEVER");        // Kitap isimleri ile islem yapmayacagim icin en basta direk buradan ekledim.Bunun disinda hersey dosyadan cekiliyor.
	strcpy(newInformation->books[1],"THE DA VINCI CODE");
	strcpy(newInformation->books[2],"THE WORLD IS FLAT");
	strcpy(newInformation->books[3],"MY LIFE SO FAR");
	strcpy(newInformation->books[4],"THE TAKING");
	strcpy(newInformation->books[5],"THE KITE RUNNER");
	strcpy(newInformation->books[6],"RUNNY BABBIT");
	strcpy(newInformation->books[7],"HARRY POTTER");
	newInformation->next = NULL;
	return newInformation;
}

calculatedSimilarities *createNodeTwo(){    //  Similarity degerlerinin tutuldugu struct yapisina gelecek olan her node icin burada yer aciyorum.
	calculatedSimilarities *newInformation = (calculatedSimilarities*)calloc(sizeof(calculatedSimilarities),1);
	newInformation->next = NULL;
	return newInformation;
}

void enqueue(bookRecommendation *newInformation){   // Queue yapisi kullandigim icin dosyadan cektigim her satiri kuyruga ekliyorum.
	if (front == NULL){        // Eger kuyruk bos ise front ve rear gelecek olan ilk nodu gosterecek.
		front = newInformation;
		rear = newInformation;
	}
	else{                     // Onun haricinde eklenecek her node bir sonraki siraya yerlesecek ve rear orayi gosterecek.
		rear->next = newInformation;
		rear = newInformation;
	}
}

void enqueue2(calculatedSimilarities *newInformation){      // Similarity degerlerinin tutuldu�u yerde de Queue yapisi kullandim.
	if (frontTwo == NULL){		// Eger kuyruk bos ise front ve rear gelecek olan ilk nodu gosterecek.
		frontTwo = newInformation;
		rearTwo = newInformation;
	}
	else{						// Onun haricinde eklenecek her node bir sonraki siraya yerlesecek ve rear orayi gosterecek.
		rearTwo->next = newInformation;
		rearTwo = newInformation;
	}
}

void readTheFile(){    // Dosyadan veri okumak icin kullandigimiz fonksiyon
	FILE *file = fopen("RecomendationDataSet.csv","r");
	if(file == NULL){
		printf("Dosya okunamadi !");
	}
	else{
		printf("Dosya okuma basarili ! \n");
		char buff[1024];           // Dosyadaki verileri tutaca��m dizi
		int rowCount = 0;      	   // Kitap isimlerinin oldu�u sat�r� atlaya��m kontrol de�i�keni
		int fieldCount;			   //  Okuma yapaca��m s�tun i�in tuttu�um saya�
		int columnCount = 0;	   // Kitap ismi icin kontrol degiskeni
		char bookNames[50][50];	   // Kitap isimleri icin dizi
		char book[50];			   // kitap isimleri gecici degisken
		bookRecommendation *newInformation;		// Struct yap�s�na aktarmak �zere olu�turdu�um ge�i�i yap�
		while(fgets(buff,1024,file)){	// sat�r sat�r okuyorum dosyay�
			
			fieldCount = 0;
			rowCount++;
			int c=0;  // kitap isimleri icin sayac degiskeni
			int d=0;  //	kitap isimlerini alirken kullanilacak sayac degiskeni
			int e=0;  // 	kitap isimlerini alirken kullanilacak sayac degiskeni
			if(rowCount == 1){     // Ilk satiri atliyor yani kitap isimlerini almiyorum.
				while(buff[c] != '\0'){
					if(buff[c] != ';' && buff[c] != '\n' && buff[c] != '\0'){    
						book[d]	= buff[c];		// Her filmi book dizisine karakter karakter dolduruyorum.
						d++;
					}
					else{
						columnCount++;
						if(columnCount != 1){     // ilk satir ilk sutundaki USERS metnini es geciyorum.
							strcpy(bookNames[e],book);
							e++;
						}
						int x;
						for(x=0;x<sizeof(book)/sizeof(char);x++){   // gecici diziyi temizliyorum.
							book[x] = '\0';
						}
						book[d] = '\0';
						d=0;	
					}
					c++;
				}		
				continue;		// ilk satiri aldiktan sonra artik bu if yapisina girmeyecek.
			}
			int i=0;
			int j=0;
			int z;
			
			char field[50];   // Kullan�c� ismi ve verilen puanlar� tutaca��m de�i�ken
			int b = -1;		  // Verilen puanlar �zerinde dola�abilmek i�in saya� de�i�keni. -1'den ba�lama sebebi ilk ba�ta Kullan�c� Ad� geldi�i i�in orda da bir kere art�yo. Puana gelince s�f�rdan ba�l�yor.	
			newInformation = createNode();    // Bellekte yer ac�yorum.
			
			while(buff[i] != '\0'){        // Okunan satirin sonuna kadar karakter karakter bakiyorum
				if(buff[i] != ';' && buff[i] != '\n' && buff[i] != '\0'){   // Noktal� virg�llerle ayr�l�yor bilgiler o y�zden 
					field[j] = buff[i];            							// ';' gelene kadar karakterleri al�yoruz.
					j++;
				}
				else{
					int newField;
				
					if(fieldCount == 0){		// �lk s�tunda kullan�c� ad� var onu al�yoruz
						strcpy(newInformation->userName,field);
					}
					if(fieldCount == 1){		// 1.Puan
						if(buff[i+1] == ';' && buff[i+1] == '\n' && buff[i+1] == ' '){    // noktal� virg�lden sonra yine noktal� virg�l,null ve bosluk gelirse
							newField = 0;		// O halde o s�tundaki de�er s�f�r olacak.
						}
						else{
							newField = atoi(field);			// Aksi halde puan� int cast edip struct a aktar�yoruz.
						}
						newInformation->givenPoint[b] = newField;
						strcpy(newInformation->books[b],bookNames[b]);
					}
					if(fieldCount == 2){	 	// 2.Puan
						if(buff[i+1] == ';' && buff[i+1] == '\n' && buff[i+1] == ' '){
							newField = 0;	
						}
						else{
							newField = atoi(field);
						}
						newInformation->givenPoint[b] = newField;
						strcpy(newInformation->books[b],bookNames[b]);
					}
					if(fieldCount == 3){		// 3.Puan
						if(buff[i+1] == ';' && buff[i+1] == '\n' && buff[i+1] == ' '){
							newField = 0;	
						}
						else{
							newField = atoi(field);
						}
						newInformation->givenPoint[b] = newField;
						strcpy(newInformation->books[b],bookNames[b]);
					}
					if(fieldCount == 4){		// 4.Puan
						if(buff[i+1] == ';' && buff[i+1] == '\n' && buff[i+1] == ' '){
							newField = 0;	
						}
						else{
							newField = atoi(field);
						}
						newInformation->givenPoint[b] = newField;
						strcpy(newInformation->books[b],bookNames[b]);
					}
					if(fieldCount == 5){		// 5.puan 
						if(buff[i+1] == ';' && buff[i+1] == '\n' && buff[i+1] == ' '){
							newField = 0;	
						}
						else{
							newField = atoi(field);
						}
						newInformation->givenPoint[b] = newField;
						strcpy(newInformation->books[b],bookNames[b]);
					}
					if(fieldCount == 6){		// 6.Puan
						if(buff[i+1] == ';' && buff[i+1] == '\n' && buff[i+1] == ' '){
							newField = 0;	
						}
						else{
							newField = atoi(field);
						}
						newInformation->givenPoint[b] = newField;
						strcpy(newInformation->books[b],bookNames[b]);
					}
					if(fieldCount == 7){		// 7.Puan
						if(buff[i+1] == ';' && buff[i+1] == '\n' && buff[i+1] == ' '){
							newField = 0;	
						}
						else{
							newField = atoi(field);
						}
						newInformation->givenPoint[b] = newField;
						strcpy(newInformation->books[b],bookNames[b]);
					}
					if(fieldCount == 8){		//	8.Puan
						if(buff[i+1] == ';' && buff[i+1] == '\n' && buff[i+1] == ' '){
							newField = 0;	
						}
						else{
							newField = atoi(field);
						}
						newInformation->givenPoint[b] = newField;
						strcpy(newInformation->books[b],bookNames[b]);
					}
				
					for(z=0;z<j;z++){    // gecici degisken olan field � temizliyoruz.
						field[z] = '\0';
					}
					field[j] = '\0';
					j=0;
					fieldCount++;		// bir sonraki s�tuna ge�ebilmek i�in fieldCount u artt�r�yoruz.
					b++;
				}
				i++;
				field[j] = '\0';
				
			}
			i = 0;
			enqueue(newInformation);		// �lk sat�r� struct'a enqueue ediyoruz.
		}
		fclose(file);
	}
}

void calculateSimilarity(char userName[70]){		// Similarity hesaplama fonksiyonu
	if (front == NULL){      // Kuyruk bo� ise Queue is clear mesaj� d�nd�r.
		printf("\n Queue is clear");
		return;
	}
	bookRecommendation * temp = front;
	bookRecommendation * temp2 = front;
	while(temp!=NULL){     // Struct icerisinde NULL gelene kadar d�n�yoruz.
		if(strcmp(temp->userName,userName) == 0){	// Parametre olarak gelen userName, struct icindeki userName ile e�le�ti�inde i�leme ba�l�yoruz.
			calculatedSimilarities *toUsers;  // gecici toUsers olustuyoruz.
			toUsers = createNodeTwo();     // 2.structta yer a��yoruz gelecek olan similarity de�erine.
			strcpy(toUsers->userName,temp->userName);
			int simCount = 0;
			while(strcmp(temp2->userName,"NU1") != 0){		// U1...U20 aras�ndaki kullan�c�lar ile similarity de�erleri olu�turuyoruz.
				int control = 0;	// kitaplara verilen puanlar� alabilmek i�in indis de�eri.
				int count=0;  // kitap say�s� kadar d�ng� kuracak de�er.
				float averageU = 0;		// U Kullan�c�s� i�in ortalama de�i�keni
				float averageN = 0;		// NU Kullan�c�s� i�in ortalama de�i�keni
				int commonCount=0;		// ortak okunan kitap say�s�
				float totalU=0;			// U kullan�c�s�n�n verdi�i toplam puan
				float totalN=0;			// NU kullan�c�s�n�n verdi�i toplam puan
				int pointsU[sizeof(temp2->givenPoint) / sizeof(int)];	// U1..U20 aras� kullan�c�lar�n, NU kullan�c�s� ile ortak okudu�u kitaplara verdi�i puanlar�n tutuldu�u dizi
				int pointsN[sizeof(temp->givenPoint) / sizeof(int)];	// NU1..NU5 aras� kullan�c�lar�n, U kullan�c�s� ile ortak okudu�u kitaplara verdi�i puanlar�n tutuldu�u dizi
				
				while(count < sizeof(temp2->givenPoint) / sizeof(int)){
					if(temp->givenPoint[control] != 0 && temp2->givenPoint[control] != 0 ){   // Her ikisinin verdi�i puan s�f�rdan farkl� ise ikiside o kitab� okumu�tur.
						totalU += temp2->givenPoint[control];		// U kullan�c�s�n�n puanlar� toplan�yor.
						totalN += temp->givenPoint[control];		// NU kullan�c�s�n�n puanlar� toplan�yor.
						pointsU[commonCount] = temp2->givenPoint[control];	// U kullan�c�s�n�n puanlar� diziye aktar�l�yor.
						pointsN[commonCount] = temp->givenPoint[control];	// NU kullan�c�s�n�n puanlar� diziye aktar�l�yor.
						commonCount++;
					}
					control++;
					count++;	
				}
				
				averageU = totalU / commonCount;     // U kullan�c�s�n�n puanlar�n�n ortalamas� hesaplan�yor.
				averageN = totalN / commonCount;	// NU kullan�c�s�n�n puanlar�n�n ortalamas� hesaplan�yor.
				int i=0;
				float pay = 0;
				float payda = 0;
				
				while(i<commonCount){
					pay += (pointsN[i]-averageN) * (pointsU[i]-averageU);    // Form�ldeki pay k�sm� hesaplan�yor.
					i++;
				}
				i=0;
				
				float x=0,y=0;
				while(i<commonCount){
					x += (pointsN[i]-averageN)*(pointsN[i]-averageN);
					y += (pointsU[i]-averageU)*(pointsU[i]-averageU);
					i++;
				}
				payda = sqrt(x*y);   // form�ldeki payda k�sm� hesaplan�yor.
				float result;
				result = pay / payda;  // sonu� bulunuyor.
				strcpy(toUsers->who[simCount],temp2->userName);
				toUsers->similarity[simCount] = result;
				simCount++;
				temp2 = temp2->next;
			}
			enqueue2(toUsers);    // similarity de�erleri ile NU ve U kullan�c�lar�da enqueue ediliyor.
			temp2 = front;
		}
		temp = temp->next;
	}
}


calculatedSimilarities *findKUsers(char userName[50]){	// Bir okuyucunun en benzer oldu�u k ki�inin belirlendi�i fonksiyon
	calculatedSimilarities *temp = frontTwo;
	while(strcmp(temp->userName,userName) != 0){   // parametre olarak gelen userName i struct i�inde bulana kadar d�n�yoruz.
		temp = temp->next;
	}
	if(strcmp(temp->userName,userName) == 0){
		printf("\n ******  %s icin similarity degerleri ****** \t\n", temp->userName);
		int i=0,j;
		float a;
		char user[50];
		while(i<sizeof(temp->similarity)/sizeof(float)){			// Similarityleri s�ralad�k ve en b�y�kleri d�nd�r�yoruz.
			for (j=i+1;j<sizeof(temp->similarity)/sizeof(float);++j)
            {
                if (temp->similarity[i] < temp->similarity[j]) 
                {
                    a =  temp->similarity[i];
            		strcpy(user,temp->who[i]);
                    temp->similarity[i] = temp->similarity[j];
                    strcpy(temp->who[i],temp->who[j]);
                    temp->similarity[j] = a;
                    strcpy(temp->who[j],user);
                }
            }
			i++;
		}	
		return temp;
	}
}

int predictBook(calculatedSimilarities *sim,int k,char userName[50]){    // Hangi kitap �nerilecek bunun belirlendi�i fonksiyon
	bookRecommendation *userN = front;
	while(strcmp(userN->userName,userName) != 0){	// parametre olarak gelen userName i struct i�inde bulana kadar d�n�yoruz.
		userN = userN->next;
	}
	int i=0;
	float averageN = 0;    // NU Kullan�c�s� i�in ortalama de�i�keni
	float totalN=0;		  // NU kullan�c�s�n�n verdi�i toplam puan 
	int notReadBooks = 0;  // NU kullan�c�s�n�n okuma�� kitap say�s�n�n de�i�keni
	int placedZero[sizeof(userN->givenPoint)/sizeof(int)];  // NU kullan�c�s�nda hangi indislerde 0 var yani okunmam�� kitap hangi indiste bunu tuttuk. U1..U20 kullan�c�lar�nda i�imize yarayacak.
	while(i<sizeof(userN->givenPoint)/sizeof(int)){  
		if(userN->givenPoint[i] == 0){
			placedZero[notReadBooks] = i;	// Okumad��� kitaplar�n indislerini buluyoruz.
			notReadBooks++;	               // Okumadigi kitap sayisi
		}  
		totalN += userN->givenPoint[i];
		i++;
	}
	averageN = totalN / (sizeof(userN->givenPoint)/sizeof(int) - notReadBooks);   // ra ortalama : 0 dan farkli olanlarin sayisina bolduk yani okunmus kitap sayisini bulduk	
	
	int a=0;
	
	printf("\n");
	
	bookRecommendation *userU = front;
	float pay=0;	// Form�ldeki pay k�sm�
	float payda = 0;  // Form�ldeki payda k�sm�
	float *result = (float*)calloc(sizeof(float),notReadBooks);  // Puan tahmin de�erlerinin tutuldu�u dizi
	char bookNames[notReadBooks][150];	// Kitap isimleri
	
	
	i=0;
	
	while(i<notReadBooks){			// Okunmayan kitap say�s� kadar d�n�yoruz
		int j=0;	
		while(j<k){					// En benzer k ki�i say�s� kadar d�n�yoruz.
			while(strcmp(userU->userName,"NU1") != 0){		// Kullan�c�lar� U1...U20 aras�nda olacak �ekilde bir d�ng� daha kuruyoruz.
				int count = 0;
				float totalU = 0;		// U kullan�c�s�n�n verdi�i toplam puan
				float averageU;			// U Kullan�c�s� i�in ortalama de�i�keni
				int countZero = 0;		// U kullan�c�s�n�n okumad��� kitap say�s� de�i�keni
				while(count < sizeof(userU->givenPoint) / sizeof(int)){   // kitap say�s� kadar d�n�p puanlar� inceliyoruz.
					totalU += userU->givenPoint[count];
					if(userU->givenPoint[count] == 0){
						countZero++;
					}
					count++;
					
				}
				averageU = totalU / (sizeof(userU->givenPoint) / sizeof(int) - countZero);    // rb ortalama : 0 dan farkli olanlarin sayisina bolduk yani okunmus kitap sayisini bulduk
				if(strcmp(userU->userName,sim->who[j]) == 0){         // Puanlar� alabilmek i�in ilk struct yap�m�za eri�tik kullan�c� ismi sorgusu yaparak.
					pay += sim->similarity[j] * (userU->givenPoint[placedZero[i]] - averageU);   // form�ldeki pay hesaplan�yor..
					payda += sim->similarity[j];		// form�ldeki payda hesaplan�yor..
					strcpy(bookNames[i],userU->books[placedZero[i]]);
				}
				userU= userU->next;
			}
			userU = front;
			j++;
		}
		result[i] = averageN + (pay/payda);  // sonu� hesapland� ve sonu� dizisine aktar�ld�.
		printf("\n'%s'---> %.3f",bookNames[i],result[i]);
		pay = 0;
		payda = 0;
		i++;
	}
	i=0;
	int j=0;
	float temporary;
	char bookName[70];
	while(i<notReadBooks){
		for (j=i+1;j<notReadBooks;++j){          // BURADA PUANI EN Y�KSEK OLANI BEL�RL�YORUZ ��NK� ONU KULLANICIYA �NERECE��Z.
            if (result[i] < result[j]) 
            {
                temporary =  result[i];
            	strcpy(bookName,bookNames[i]);
                result[i] = result[j];
                strcpy(bookNames[i],bookNames[j]);
                result[j] = temporary;
                strcpy(bookNames[j],bookName);
            }
        }
		i++;
	}
	printf("\n\n Onerilecek Kitap : %s",bookNames[0]);    // En y�ksek puan� olan kitap �nerildi.
}


int main(){
	readTheFile();    // Dosya okuduk.
	char userName[70];		// Kullan�c�n�n girece�i kullan�c� ad� : NU1...NU5
	printf("Kitap onerisi yapilacak kullanici adi : ");
	scanf("%s",userName);
	calculateSimilarity(userName);  // Similarity de�erlerini hesaplad���m�z fonksiyon
	int k;  // Bir okuyucunun en benzer oldu�u k ki�inin belirlenmesi girilecek olan k de�eri
	printf("\n\nEn benzer k kisi icin k degeri giriniz : ");
	scanf("%d",&k);
	
	calculatedSimilarities *temp = findKUsers(userName);    // Bir okuyucunun en benzer oldu�u k ki�inin belirlendi�i fonksiyon
	int i=0;
	while(i<k){
		printf("\n%s : %.3f",temp->who[i],temp->similarity[i]);    // En benzer k ki�iyi yazd�r�yoruz similarity de�erleri ile birlikte.
		i++;
	}
	
	predictBook(temp,k,userName);   // Son olarak hangi kitap �nerilecek bunun belirlendi�i fonksiyon
	
	return 0;

}
