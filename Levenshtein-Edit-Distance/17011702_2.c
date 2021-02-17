#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TABLE_SIZE 999983  // 1000000'den k���k en b�y�k asal say�y� tablo boyutu olarak ald�k.

typedef struct link{
	unsigned long long int key; // Key de�eri int'in alan boyunu ge�ti�i i�in unsigned long long int tan�mlad�m.
	char word[50];  // Kelimeleri tuttu�um yer.
}hashTable;

typedef struct link2{
	unsigned long long int key;
	char hataliKelime[50];   // Hatal� kelime tuttu�um yer
	char onerilenKelime[50]; // �nerilen kelimeyi hatal� kelimenin yan�na eklersek bir sonraki hatal� kelime aray���m�zda �nceden hata ald���m�z de�er gelirse yan�ndaki �nerilen kelimeyi direk ��kt� olarak verecek.
}hataliKelimeTablosu;

hashTable hash[TABLE_SIZE];    // Hash Tablosu
hataliKelimeTablosu hash2[TABLE_SIZE];  // Hatal� Kelime Tablosu

void baslangicKeyDegeri(){   // Tabloda key de�erlerini ilk ba�ta -1 olarak i�aretleme yapaca��m fonksiyon.
	int i;
	for(i=0;i<TABLE_SIZE;i++){ 
		hash[i].key = -1;  //  B�ylelikle i�inde de�er olmayan sat�rlar -1 olarak g�sterilmi� oldu.
		hash2[i].key = -1;
	}
}

void ekleHashTable(int key,char word[]){  // Tabloya yerle�tirilecek olan bilgilerin i�lendi�i fonksiyon.
	int i = 0;
	int ilkHash = key % TABLE_SIZE;  // Double hashing'de iki kez hash'den ge�irmemiz gerekiyor key de�erini
	int ikinciHash = 1 + (key % (TABLE_SIZE - 1) ); // �kinci hash'den ge�irdik.
	int calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE;  //  i = 0 de�erine g�re key i�in gerekli indis bulundu.
	
	if (hash[calculatedHash].key == -1){  // E�er gelen key de�eri i�in belirlenen indiste -1 mevcutsa o zaman ilk defa o indise key atanacakt�r.
		hash[calculatedHash].key = key;
		strcpy(hash[calculatedHash].word,word);
	}
	else{// else durumunda ise hash tablosunda belirlenen indiste -1 yok yani bu durum bize daha �nce o indise bir key de�eri yerle�mi� oldu�unu belirtiyor.
		while(i<TABLE_SIZE){ // i de�eri tablo boyutunu a�mayacak �ekilde;
		  //  gelen key de�eri -1'den farkl� m� , gelen key de�eri i�in daha �nce ayn� indiste bir key yerle�mi� mi  kontrollerini yapar�z.
			if(strcmp(hash[calculatedHash].word, word) != 0){
				calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE; // b�ylelikle kendisine bo� indis bulana dek bu d�ng� i sayac�m�z sayesinde devam eder.
				if (hash[calculatedHash].key == -1 ) 
			    {
					hash[calculatedHash].key = key;  // tabloya yerle�tiriliyor.
					strcpy(hash[calculatedHash].word,word);
				}
			}
			i++;
		}
	}
}

void dokumanOku(){  // D�k�mandan gelen kelimeleri okuyup kelimeler i�in horner metoduna g�re key de�eri olu�turup bunu ekleHashTable fonksiyonuna yollay�p hashtable �zerinde gerekli indise yerle�tiriyoruz.
	FILE *fp;
	if((fp = fopen("smallDictionary.txt","r"))== NULL){   // Dosya okuma kontrol�
		printf("Dosya acma hatasi");
	}
	int i=0,j;
	unsigned long long int key=0;  // key de�eri int de�erini a�aca�� i�in unsigned long long int yapt�m.
	char temp[50];   // D�k�mandaki t�m karakterleri kontrol olarak kullanmak i�in alaca��m ge�ici dizi
	char temp2[50];  // D�k�mandaki t�m kelimeleri alaca��m ge�ici dizi
	while(!feof(fp)){
		temp[i] = fgetc(fp);
		if(temp[i] == ' ' || temp[i] == '\n'){   // D�k�manda kelimeler aras�nda bo�luk (' ') olaca�� i�in kontrol yap�yorum her bo�lukta kelimeyi al�p hash tablosuna aktar�yorum.
			key = hornerMetodu(temp2);
			key = abs(key);
			if (key != 0){
				ekleHashTable(key,temp2);  // Gelen key de�eri,kelime ve dosyaAdina g�re Hash tablosuna eklenmek �zere ekleHashTable fonksiyonuna yollan�yor.
			}
				
			for(j=0;j<50;j++){  // Gecici temp2 degiskeninin i�ini bo�alt�yoruz.
				temp2[j] = '\0';
			}	
			i=0;
		}
		else{
			temp2[i] = temp[i];  // temp ile son i�lemde her zaman �zel karakter gelece�i i�in kelimeyi temp2'de tutuyorum.
			i++;
		}
	}
	fclose(fp);
}

int kelimeAra(char word[]){  // Bu fonksiyon sayesinde aranan kelime hash tablosunda var m� ? e�er var ise hangi d�k�manlarda yer ald���n� kullan�c�ya ��kt� olarak verecek.
	int i=0,j;
	unsigned long long int key;  // key de�eri int de�erini a�aca�� i�in unsigned long long int yapt�m.
	key = hornerMetodu(word);
	int ilkHash = key % TABLE_SIZE;  // Double hashing'de iki kere hash'den ge�irip indis de�eri �retece�imiz i�in
	int ikinciHash = 1 + (key % (TABLE_SIZE - 1) ); // �kinci hash i�lemi
	int calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE; // Ve indis de�eri olu�tu.
	int mesafeBirVarmi=0;
	int mesafeIkiVarmi=0;
	if(hash[calculatedHash].key == key && strcmp(hash[calculatedHash].word, word) != 1){   // �lgili indiste bulunan key de�eri ile kullan�c�n�n g�ndermi� oldu�u kelimenin key de�eri birbirine e�it mi ? 
		printf("\nKelime hash tablosunda zaten mevcut : %s",hash[calculatedHash].word);
		return 1;
	}
	else{
		printf("\n");
		int distance; // LevenshteinEditDistance fonksiyonundan d�necek olan Distance(Mesafe) de�eri.
		for(i=0;i<TABLE_SIZE;i++){
			if(hash[i].key != -1){  
				distance = LevenshteinEditDistance(word,hash[i].word); // Tablodaki t�m kelimeler, klavyeden girilen kelime ile mesafesi bulunmak �zere LevenshteinEditDistance fonksiyonuna g�nderilir.
				if(distance == 1){  // Mesafesi 1 olanlar
					printf("\n'%s' icin bu kelimeyi mi aradiniz (Mesafe %d) : %s",word,distance,hash[i].word);
					mesafeBirVarmi = 1; // Mesafe 1 olan kelimeler var ise ekranda onlar g�sterilmeli bu y�zden kontrol i�areti koyuyoruz.
				}
			}
		}
		if(mesafeBirVarmi != 1){ // E�er mesafeBirVarmi != 1 ise o zaman mesafe 1 olan kelime hi� yoktur.
			for(i=0;i<TABLE_SIZE;i++){
				if(hash[i].key != -1){
					distance = LevenshteinEditDistance(word,hash[i].word);  // Yine tablodaki t�m kelimeler ile klavyeden girilen kelimelerin mesafesi hesaplan�r.
					if(distance == 2){  // Mesafesi 2 olanlar
						printf("\n'%s' icin bu kelimeyi mi aradiniz (Mesafe %d) : %s",word,distance,hash[i].word);
						mesafeIkiVarmi = 1; // Mesafe 1 olmay�p, Mesafe 2 olan kelimeler var ise ekranda onlar g�sterilmeli bu y�zden kontrol i�areti koyuyoruz.
					}
				}
			}			
		}
		if(mesafeBirVarmi == 0 && mesafeIkiVarmi == 0){ // E�er Mesafe 1 ve Mesafe 2 i�in kelime bulunamazsa o halde aranan kelime hem hash tablosunda yok hem de mesafesi 2den b�y�kt�r.
			return 2;
		}
	}
		
}

int dogruKelimeyiAra(char word[]){  // Bu fonksiyon sayesinde aranan kelime hash tablosunda var m� ? e�er var ise hangi d�k�manlarda yer ald���n� kullan�c�ya ��kt� olarak verecek.
	int i=0,j;
	unsigned long long int key;  // key de�eri int de�erini a�aca�� i�in unsigned long long int yapt�m.
	key = hornerMetodu(word); // kelimeye ait key de�eri bulmak i�in horner metodundan ge�irdik.
	int ilkHash = key % TABLE_SIZE;  // Double hashing'de iki kere hash'den ge�irip indis de�eri �retece�imiz i�in
	int ikinciHash = 1 + (key % (TABLE_SIZE - 1) ); // �kinci hash i�lemi
	int calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE; // Ve indis de�eri olu�tu.
	if(hash[calculatedHash].key == key && strcmp(hash[calculatedHash].word, word) != 1){   // �lgili indiste bulunan key de�eri ile kullan�c�n�n g�ndermi� oldu�u kelimenin key de�eri birbirine e�it mi ? 
		return 1; //  1 : Kelime var
	}
}

int hornerMetodu(char word[]){
	int i=0,j,k=0;
	unsigned long long int key=0;  // key de�eri int de�erini a�aca�� i�in unsigned long long int yapt�m.
	int length = strlen(word); 
	int power = length-1; //  Horner metodunda R^(Length-1) alaca�� i�in �s de�erini belirliyoruz ve d�ng�de 0 a kadar azalacak �ekilde i�leme sokuyoruz.
	char gecici;
	int fark = 'a' - 'A';  //ASCII tablosunda k���k harfler ve b�y�k harfler aras�ndaki fark 32.
	while (word[k] != '\0') {
        if (word[k] >= 'A' && word[k] <= 'Z') { //harf b�y�k ise k���lt�yoruz..
            word[k] += fark;  // fark kadar eklersek bu harfin ASCII degeri b�y�r ve ayn� harfin k�����ne sahip olmu� olur.
        }
        k++;
    }
	for(j=0;j<length;j++){
		gecici = word[j];
		if (gecici >= 'a'){  // Araba ve araba ayn� kelimeler gibi d���nece�iz bu nedenle kontrol� ger�ekle�tirdik.
			gecici = 'A' - 'a' + gecici;
		}	
		key += abs((pow(31,power)*(gecici-'A'+1)));  // Horner metodunda gelen kelimenin karakter say�s�n� kullanarak d�ng�ye sokuyoruz i�lem bitti�inde kelime i�in key de�eri olu�acakt�r.
		power--;
	}
	key = abs(key);
	return key;  // key de�erini d�nd�r�yoruz.
}

int hataliTablodaArama(int key,char kelime[]){ // Hatal� tabloda arama fonksiyonu
	int i=0;
	key = abs(key);
	int ilkHash = key % TABLE_SIZE;  // Double hashing'de iki kere hash'den ge�irip indis de�eri �retece�imiz i�in
	int ikinciHash = 1 + (key % (TABLE_SIZE - 1) ); // �kinci hash i�lemi
	int calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE; // Ve indis de�eri olu�tu.
	if(hash2[calculatedHash].key == key && strcmp(hash2[calculatedHash].hataliKelime, kelime) != 1){   // �lgili indiste bulunan key de�eri ile kullan�c�n�n g�ndermi� oldu�u kelimenin key de�eri birbirine e�it mi ? 
		printf("\n\n'%s' kelimesi HATALI KELIMELER TABLOSUnda mevcut. Bu kelime icin daha once kabul ettiginiz kelime : '%s'",hash2[calculatedHash].hataliKelime,hash2[calculatedHash].onerilenKelime);
		return 1; // Daha �nce kelimenin hatal� kelime tablosunda var oldu�unu belirten kontrol de�eri.
	}
	else{
		return 0;
	}
}

void ekleHataliTablo(int key,char hataliKelime[],char onerilenKelime[]){
	int i = 0;
	int ilkHash = key % TABLE_SIZE;  // Double hashing'de iki kez hash'den ge�irmemiz gerekiyor key de�erini
	int ikinciHash = 1 + (key % (TABLE_SIZE - 1) ); // �kinci hash'den ge�irdik.
	int calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE;  //  i = 0 de�erine g�re key i�in gerekli indis bulundu.
	
	if (hash2[calculatedHash].key == -1){  // E�er gelen key de�eri i�in belirlenen indiste -1 mevcutsa o zaman ilk defa o indise key atanacakt�r.
		hash2[calculatedHash].key = key;
		strcpy(hash2[calculatedHash].hataliKelime,hataliKelime);
		strcpy(hash2[calculatedHash].onerilenKelime,onerilenKelime);
	}
	else{// else durumunda ise hash tablosunda belirlenen indiste -1 yok yani bu durum bize daha �nce o indise bir key de�eri yerle�mi� oldu�unu belirtiyor.
		while(i<TABLE_SIZE){ // i de�eri tablo boyutunu a�mayacak �ekilde;
		//  gelen key de�eri -1'den farkl� m� , gelen key de�eri i�in daha �nce ayn� indiste bir key yerle�mi� mi  kontrollerini yapar�z.
			if(strcmp(hash2[calculatedHash].hataliKelime, hataliKelime) != 0){
				calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE; // b�ylelikle kendisine bo� indis bulana dek bu d�ng� i sayac�m�z sayesinde devam eder.
				if (hash2[calculatedHash].key == -1 ) 
			    {
					hash2[calculatedHash].key = key;
					strcpy(hash2[calculatedHash].hataliKelime,hataliKelime);
					strcpy(hash2[calculatedHash].onerilenKelime,onerilenKelime);
				}
			}
			i++;
		}
		
	}
}

int enKucukBul(int x,int y, int z){ // Insert , Delete ve Change i�lemleri i�in en k����� bulan fonksiyon
	if(x<=y && x<=z){
		return x;
	}
	else if(y<=x && y<=z){
		return y;
	}
	else{
		return z;
	}
}


int LevenshteinEditDistance(char ilkKelime[],char ikinciKelime[]){  // Mesafe buldu�umuz fonksiyon
	
	int i,j;
	int ilkKelimeLength = strlen(ilkKelime); // 1.kelime karakter say�s�
	int ikinciKelimeLength = strlen(ikinciKelime); // 2.kelime karakter say�s�
	int insert,delete,change,copy;
	char temp,temp2;
	
	int matris[ilkKelimeLength+1][ikinciKelimeLength+1]; // kelimelerin 1 fazlas�n�n olma sebebi en ba�ta (0x0) bo� karakter olarak almam�z.
	
	for(i=0;i<=ikinciKelimeLength;i++){
		matris[0][i] = i;	// �lk Sat�r� initialize ettik bo� karakterlere olan uzakl�klar�n� kullanarak.
	}
	for(i=0;i<=ilkKelimeLength;i++){
		matris[i][0] = i;	// �lk S�tunu initialize ettik bo� karakterlere olan uzakl�klar�n� kullanarak.
	}
	
	for(i=1;i<=ilkKelimeLength;i++){  // Matriste ilgili indislere Mesafe'leri eklemek i�in geziniyoruz.
		temp = ilkKelime[i-1];  // i=1'den ba�lad���m�z i�in i-1 yapt�k.
		for(j=1;j<=ikinciKelimeLength;j++){
			temp2 = ikinciKelime[j-1]; // j=1'den ba�lad���m�z i�in j-1 yapt�k.
			if(temp == temp2){
				copy = matris[i-1][j-1];  // Ayn� harf gelirse sol �apraz �st�ndeki Mesafe'yi alabiliriz. Copy i�leminde ayn� harf oldu�u i�in COST olmaz.
				matris[i][j] = copy;
			}
			else{
				insert = matris[i-1][j]+1; // Insert i�in Mesafe i�lemi
				delete = matris[i][j-1]+1; // Delete i�in Mesafe i�lemi
				change = matris[i-1][j-1]+1; // Change i�in Mesafe i�lemi
				matris[i][j] = enKucukBul(insert,delete,change);  // Insert, Delete ve Change'den d�nen Mesafe'lerden en k�����n� matris[i][j] ye ekleyece�iz.
				if(matris[i][j] > 2 && (( ilkKelimeLength-ikinciKelimeLength + j) == i)){  // ****** BONUS ********  Alt sat�rlarda a��klad�m ama raporda daha detayl� a��klad�m.
					// enKucukBul fonksiyonundan d�nen de�er 2'den b�y�k VE 
					// mevcut indis yani iki kelimenin fark�n�n j ile toplam� i'ye e�it ise o zaman matris[ilkKelimeLength][ikinciKelimeLength] indisine diyagonal olan indislerde 2'den b�y�k bir mesafe de�eri yerle�ti.
					return -1; 
				}
			}
			temp2 = '\0'; // ge�ici karakteri s�f�rlad�k.
		}
		temp = '\0'; // ge�ici karakteri s�f�rlad�k.
	}
	return matris[ilkKelimeLength][ikinciKelimeLength]; // Matrisin en sa� ve en altta olan indisi bize iki kelimenin birbirine Distance'�n� verecek.
}


int main(){
	baslangicKeyDegeri();   // Hash tablosunda ba�lang��ta t�m KEY de�erlerini -1 olarak i�aretliyoruz.
	dokumanOku();  // smallDictionary s�zl���ndeki kelimeler hash tablosuna �ekilir.
	while(1){
		char cumle[200];
		printf("\nCumle giriniz : ");
		fflush(stdin); // bilgisayar�mda gets d�zg�n �al��m�yordu internetten aratt���mda bunun eklenmesi ile sorun ��z�l�yor yaz�yordu ve ekledim �al��t�.
		gets(cumle); // Girilecek c�mle
		char temp[80]; // C�mleleri kelimelere ay�r�p burada tutup bu dizi �zerinden i�lem yapaca��z.
		
		int len = strlen(cumle);
		int i = 0;
		while(i < len){  // C�mle karakter say�s� kadar d�ng� kurulacak.
			int j = 0;
			while(i < len && cumle[i] != ' '){ // Her kelime aras�nda bo�luk var bu y�zden bo�luk kontrol� yapt�k.
				temp[j] = cumle[i];  // C�mledeki her karakteri temp'e at�yoruz.
				j++;
				i++;
			}
			i++;
			temp[j] = '\0';
			int key; // hatali kelimenin key de�eri
			key = hornerMetodu(temp); // kelime icin key de�eri �retiyoruz horner fonksiyonu ile
			if(hataliTablodaArama(key,temp) == 1){ // Daha �nceden hatal� tabloda varm� ?
				// print islemini 	hataliTablodaArama() icinde yapt�g�m icin buradan i�lem yapmama gerek kalmad�.
			}
			else{
				char dogruKelime[50]; // onerilen kelimeler i�in girece�imiz dogru kelime
				switch(kelimeAra(temp)){  // kelime arama fonksiyonuyla ger�ekle�ecek olan i�lem �zerinden d�nen return de�erine g�re switch kontrol� yap�yoruz.
					case 1: break;  // Kelime hash tablosunda mevcut ��kt�s� verir.
					case 2: printf("\n'%s' kelimesi HASH TABLOSUnda yok ayrica onerilen kelimede bulunamadi !\n",temp); break;
					default :
							printf("\n\nDogru kelimeyi secin : ");
							scanf("%s",dogruKelime);
							if(dogruKelimeyiAra(dogruKelime) == 1){  // Onerilenlere g�re kullan�c�n�n girdi�i dogru kelimeyi yeniden hash tablosunda ar�yoruz.
								ekleHataliTablo(key,temp,dogruKelime); // varsa onerilenlerden do�ru kelime girmi�tir ve hatali tabloya ekleme yap�yoruz.
							}
							else{
								printf("\nOnerilen kelimelerin disinda bir secim yaptiniz !(Cunku hash tablosunda girdiginiz kelime yok !)\n");
							}
							break;
				}		
			}
			for(j=0;j<80;j++){  // kelimeyi tuttugum temp'i bir sonraki kelimeyi alaca��mdan dolay� temizliyorum.
				temp[j] = '\0';
			}		
		}
	}
	
	return 0;
}
