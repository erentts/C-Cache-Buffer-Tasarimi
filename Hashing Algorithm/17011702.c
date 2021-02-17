#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TABLE_SIZE 997  // 1000'den k���k en b�y�k asal say�y� tablo boyutu olarak ald�k.

typedef struct link{
	unsigned long long int key; // Key de�eri int'in alan boyunu ge�ti�i i�in unsigned long long int tan�mlad�m.
	char word[50];  // Kelimeleri tuttu�um yer.
	char dosyaAdi[50][50];  // Dosya isimlerini tuttu�um yer. 
}hashTable;

hashTable hash[TABLE_SIZE];    // Hash Tablosu

void baslangicKeyDegeri(){   // Tabloda key de�erlerini ilk ba�ta -1 olarak i�aretleme yapaca��m fonksiyon.
	int i;
	for(i=0;i<TABLE_SIZE;i++){ 
		hash[i].key = -1;  //  B�ylelikle i�inde de�er olmayan sat�rlar -1 olarak g�sterilmi� oldu.
	}
}

double tablodakiElemanSayisi(){   // Load Factor hesaplarken bizim i�in gerekli olan hash tablosundaki eleman say�s�n�(N) buluyoruz.
	int i;
	double sayac=0;
	for(i=0;i<TABLE_SIZE;i++){
		if(hash[i].key != -1){  // -1 olmayan t�m keylerde kelimeler var bu y�zden -1 key de�eri olmayanlar�n say�s�n� alabiliriz.
			sayac++;
		}
	}
	return sayac;  // Tablodaki Eleman Say�s� D�ner.
}

double calculateLoadFactor(){   // LoadFactor = Tablodaki Eleman Say�s� / Tablo Boyutu ; L = N / M 
	int tableSize = TABLE_SIZE;  // M -> Tablo Boyutu
	double tabloElemanSayisi = (double)tablodakiElemanSayisi();  // N -> Tablodaki Eleman Say�s�
	double loadFactor = (double)(tabloElemanSayisi / tableSize); // LoadFactor = N / M
	return loadFactor;
}

void ekleHashTable(int key,char word[],char dosyaAdi[]){  // Tabloya yerle�tirilecek olan bilgilerin i�lendi�i fonksiyon.
	int i = 0;
	int ilkHash = key % TABLE_SIZE;  // Double hashing'de iki kez hash'den ge�irmemiz gerekiyor key de�erini
	int ikinciHash = 1 + (key % (TABLE_SIZE - 1) ); // �kinci hash'den ge�irdik.
	int calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE;  //  i = 0 de�erine g�re key i�in gerekli indis bulundu.
	
	if (hash[calculatedHash].key == -1){  // E�er gelen key de�eri i�in belirlenen indiste -1 mevcutsa o zaman ilk defa o indise key atanacakt�r.
		hash[calculatedHash].key = key;
		strcpy(hash[calculatedHash].word,word);
		strcpy(hash[calculatedHash].dosyaAdi[0],dosyaAdi);	
	}
	else{// else durumunda ise hash tablosunda belirlenen indiste -1 yok yani bu durum bize daha �nce o indise bir key de�eri yerle�mi� oldu�unu belirtiyor.
		while((hash[calculatedHash].key != -1) && hash[calculatedHash].key != key && i<TABLE_SIZE){ // i de�eri tablo boyutunu a�mayacak �ekilde;
			i++;  //  gelen key de�eri -1'den farkl� m� , gelen key de�eri i�in daha �nce ayn� indiste bir key yerle�mi� mi  kontrollerini yapar�z.
			calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE; // b�ylelikle kendisine bo� indis bulana dek bu d�ng� i sayac�m�z sayesinde devam eder.
		}
		i=0;
		
		while(strlen(hash[calculatedHash].dosyaAdi[i]) > 0){    // Gelen key de�eri ve kelime daha �nce varsa �st�ne yaz�l�r, fakat;
			i++;												// farkl� d�k�mandan geldiyse dosya isimleri s�ras� ile eklenir.
		}														// Bunun i�in ayn� dosya ismi 2.kez geldi�inde onu yazd�rmamak ad�na
		int kontrol=0,j;										// bir kontrol yapmam�z gerekiyor bu nedenle �nce ka� tane dosya varsa say�s�n� al�yorum.
		for(j=0;j<i;j++){  // dosya say�s� kadar d�ng� ger�ekle�tiriyorum.
			if(strcmp(hash[calculatedHash].dosyaAdi[j],dosyaAdi)== 0){  // Yeni gelen dosya daha �nce eklenmi� mi onun sorgusunu yap�p bir kontrol de�eri olu�turuyorum.
				kontrol = 1;
			}
		}
		if(kontrol == 0){   // kontrol = 1 ise ayn� dosya daha �nce kaydedilmi� fakat kontrol = 0 ise farkl� dosya gelmi� ve kaydedebiliriz.
			hash[calculatedHash].key = key;
			strcpy(hash[calculatedHash].word,word);
			strcpy(hash[calculatedHash].dosyaAdi[i],dosyaAdi);	
		}
	}
}

void oncedenHashTablosuVarmi(){  // �lk d�k�man� kulland���m�zda bir hash tablosu olu�acak fakat sonradan gelen d�k�manlar i�in �nceki hash tablosunuda kullanaca��z.
	FILE *fp;
	if((fp = fopen("17011702.txt","r"))!= NULL){ // Daha �nce hi� hash tablosu olu�mu� mu bunun sorgusunu yap�yoruz.
		printf("\nDaha once olusturulan bir hash tablosu vardi simdi yeni dokuman ismi giriniz...");
		hashTable newData;
		char temp[50];  // Sayfadaki t�m karakterleri kontrol etmek �zere kullanaca��m dizi
		char temp2[50];  // Kelime , key ve dosya ad�n� hashtablosuna tekrar yollarken kullanaca��m ge�ici dizi
		char key[50]; // temp2 dizisinden gelen key de�erini bu key dizisine ataca��m daha sonra bunu integer format�na d�n��t�r�yor olaca��m.
		char word[50]; // temp2 dizisinden gelen word de�erini bu word dizisine ataca��m.
		char dosyaAdi[50]; // temp2 dizisinden gelen word de�erini bu dosyaAdi dizisine ataca��m.
		int sira=0;  // key , kelime ve dosya ad� i�lemlerinin hangisi temp2 dizisine geldiyse kontrol sayac� olarak kullanaca��m de�i�ken.
		int i=0,j;
		int newKey;  // dosyadan okurken char tipinde olan key de�erini tekrar integer format�na d�n��t�rece�iz ve o de�eri bu de�i�kende tutaca��z.
		while(!feof(fp)){  // Dosya sonuna kadar okuma yap�yoruz.
			temp[i] = fgetc(fp);
			if(temp[i] == '/' || temp[i] == '\n'){  // Dosyada, key/kelime/dosyaAd1 dosyaAdi2 dosyaAdi 3 �eklinde tutuldu�u i�in '/' ve '\n' sorgusu yap�yorum.
				if(sira == 0){  // sira == 0 ise KEY de�eri gelmi�tir ve key dizisinde sakl�yoruz.
					strcpy(key,temp2);
					sira++;
					for(j=0;j<50;j++){  // daha sonra temp2 de�i�kenini yine kullanaca��m i�in i�indeki karakterleri siliyorum.
						temp2[j] = '\0';
					}
					i=0;
				}
				else if(sira == 1){   // sira == 1 ise KEL�ME gelmi�tir ve word dizisinde sakl�yoruz.
					strcpy(word,temp2);
					sira++;
					for(j=0;j<50;j++){ // daha sonra temp2 de�i�kenini yine kullanaca��m i�in i�indeki karakterleri siliyorum.
						temp2[j] = '\0';
					}
					i=0;
				}
				else{      // sira = 3 olmu�tur ve son i�lem olan dosya isimlerini aktarma ve daha �nce word ve key dizisinde tuttuklar�m�z�da tekrar tabloya aktaraca��z.
					strcpy(dosyaAdi,temp2); // temp2 deki dosya ad�n� dosyaAdi dizisine aktard�k.
					newKey = atoi(key); // key de�erinde char olarak tutulan de�eri integer olarak d�zenledik.
					ekleHashTable(newKey,word,dosyaAdi);  // ekleHashTable fonksiyonuna key de�erini belirleyip hash tablosuna aktarmak �zere yollad�k.
					sira = 0;
					for(j=0;j<50;j++){
						temp2[j] = '\0';  // her seferinde yeni de�erler i�in kullanaca��m�z ge�ici temp2 de�i�keninin i�indekileri siliyoruz.
					}
					for(j=0;j<50;j++){
						key[j] = '\0';   // her seferinde yeni de�erler i�in kullanaca��m�z ge�ici key de�i�keninin i�indekileri siliyoruz.
						word[j] = '\0';   // her seferinde yeni de�erler i�in kullanaca��m�z ge�ici word de�i�keninin i�indekileri siliyoruz.
						dosyaAdi[j] = '\0';   // her seferinde yeni de�erler i�in kullanaca��m�z ge�ici dosyaAdi de�i�keninin i�indekileri siliyoruz.
					}
					i=0;
					newKey = 0;
				}
			}
			else{
				temp2[i] = temp[i];  // temp ile son i�lemde her zaman �zel karakter gelece�i i�in key,kelime ve dosya ad�n� temp2'de tutuyorum.
				i++;
			}
		}
	}
}

void dosyayaYaz(double loadFactor){   // Hash Tablosunu dosyaya yazd���m�z fonksiyon
	FILE *fp;
	if((fp = fopen("17011702.txt","w+")) == NULL){  
		printf("Dosya acma hatasi");
	}
	
	int i,j=0;
	for(i=0;i<TABLE_SIZE;i++){
		if(hash[i].key != -1){     // -1 de�erine e�it olan key'lere bilgi gelmemi�tir ve onlar� haricinde olan t�m bilgileri d�nd�rebiliriz.
			fprintf(fp,"%d/%s/",hash[i].key,hash[i].word);  // her gelen sat�r i�in key, word de�erini yazd�r�r�z.
			j=0;
			while(strlen(hash[i].dosyaAdi[j]) > 0){  // dosya ad�nda ise 1 den fazla dosya ad� ismi olabilece�i i�in onuda kendi i�erisinde d�ng�ye sokuyoruz.
				fprintf(fp,"%s",hash[i].dosyaAdi[j]);
				j++;
			}
			fprintf(fp,"%c\n",' ');
		}
	}
	fprintf(fp," Load Factor  = %.1f",loadFactor);
	fclose(fp);
}

void dokumanOku(){  // D�k�mandan gelen kelimeleri okuyup kelimeler i�in horner metoduna g�re key de�eri olu�turup bunu ekleHashTable fonksiyonuna yollay�p hashtable �zerinde gerekli indise yerle�tiriyoruz.
	char dosyaAdi[50];
	printf("\nAcilacak dosya adi giriniz ( Ornek : dosyaAdi.txt ) : ");
	scanf("%s",dosyaAdi);
	FILE *fp;
	if((fp = fopen(dosyaAdi,"r"))== NULL){   // Dosya okuma kontrol�
		printf("Dosya acma hatasi");
	}
	int i=0,j;
	double loadFactor; // loadFactor de�i�keni
	unsigned long long int key=0;  // key de�eri int de�erini a�aca�� i�in unsigned long long int yapt�m.
	char temp[50];   // D�k�mandaki t�m karakterleri kontrol olarak kullanmak i�in alaca��m ge�ici dizi
	char temp2[50];  // D�k�mandaki t�m kelimeleri alaca��m ge�ici dizi
	while(!feof(fp)){
		temp[i] = fgetc(fp);
		if(temp[i] == ' ' || temp[i] == '\n'){   // D�k�manda kelimeler aras�nda bo�luk (' ') olaca�� i�in kontrol yap�yorum her bo�lukta kelimeyi al�p hash tablosuna aktar�yorum.
			int length = strlen(temp2);          // Ayr�ca sat�r sonuna gelince ('\n') sat�rdaki son kelimeyide al�p alt sat�r i�in i�leme devam eder.
			int power = length-1;  //  Horner metodunda R^(Length-1) alaca�� i�in �s de�erini belirliyoruz ve d�ng�de 0 a kadar azalacak �ekilde i�leme sokuyoruz.
			key = 0;
			char gecici;
			for(j=0;j<length;j++){
				gecici = temp2[j];
				if (gecici >= 'a'){     // Araba ve araba ayn� kelimeler gibi d���nece�iz bu nedenle kontrol� ger�ekle�tirdik.
					gecici = 'A' - 'a' + gecici;
				}
				key += abs((pow(3,power)*(gecici-'A'+1)));   // Horner metodunda gelen kelimenin karakter say�s�n� kullanarak d�ng�ye sokuyoruz i�lem bitti�inde kelime i�in key de�eri olu�acakt�r.
				power--;		
			}	
			loadFactor = calculateLoadFactor(); // calculateLoadFactor fonksiyonu ile Load Factor say�s�n� �ektik.
			printf("LoadFactor : %.1f\n",loadFactor);
			if(loadFactor == 1.0 || loadFactor > 1.0){  // E�er LoadFactor >= 1.0 ise Ekleme ��lemi Duracak ve Eklenemeyen Kelimeler Ekrana Yaz�lacak.
				printf("Eklenemeyen Kelime : %s\n",temp2);
				for(j=0;j<50;j++){ // temp2 daha sonraki kelimelerde kullanaca��m�z i�in i�ini bo�altt�k.
					temp2[j] = '\0';
				}
				i=0;
			}
			else{
				if(loadFactor > 0.8){   // Load Factor > 0.8 'den 0.8 de�erini ge�ti�imizi bildirmek i�in kullan�c�ya uyar� mesaj� verecek.
					printf("LOAD FACTOR 0.8 degerini gecti\n");
				}
			
				if (key != 0){
					ekleHashTable(key,temp2,dosyaAdi);  // Gelen key de�eri,kelime ve dosyaAdina g�re Hash tablosuna eklenmek �zere ekleHashTable fonksiyonuna yollan�yor.
				}
				
				for(j=0;j<50;j++){  // Gecici temp2 degiskeninin i�ini bo�alt�yoruz.
					temp2[j] = '\0';
				}	

				i=0;
			}
		}
		else{
			temp2[i] = temp[i];  // temp ile son i�lemde her zaman �zel karakter gelece�i i�in kelimeyi temp2'de tutuyorum.
			i++;
		}
	}
	dosyayaYaz(loadFactor);  // Daha sonra dosyaya yazd�r�yorum olu�an hash tablosunu ve loadfactor de�erinide yolluyorum.
	fclose(fp);
}

void kelimeAra(char word[]){  // Bu fonksiyon sayesinde aranan kelime hash tablosunda var m� ? e�er var ise hangi d�k�manlarda yer ald���n� kullan�c�ya ��kt� olarak verecek.
	int i=0,adimSayisi=1,j;
	unsigned long int key=0;  // key de�eri int de�erini a�aca�� i�in unsigned long long int yapt�m.
	int length = strlen(word); 
	int power = length-1; //  Horner metodunda R^(Length-1) alaca�� i�in �s de�erini belirliyoruz ve d�ng�de 0 a kadar azalacak �ekilde i�leme sokuyoruz.
	char gecici;
	for(j=0;j<length;j++){
		gecici = word[j];
		if (gecici >= 'a'){  // Araba ve araba ayn� kelimeler gibi d���nece�iz bu nedenle kontrol� ger�ekle�tirdik.
			gecici = 'A' - 'a' + gecici;
		}	
		key += abs((pow(3,power)*(gecici-'A'+1)));  // Horner metodunda gelen kelimenin karakter say�s�n� kullanarak d�ng�ye sokuyoruz i�lem bitti�inde kelime i�in key de�eri olu�acakt�r.
		power--;
	}
	int ilkHash = key % TABLE_SIZE;  // Double hashing'de iki kere hash'den ge�irip indis de�eri �retece�imiz i�in
	int ikinciHash = 1 + (key % (TABLE_SIZE - 1) ); // �kinci hash i�lemi
	int calculatedHash = (ilkHash + i * ikinciHash) % TABLE_SIZE; // Ve indis de�eri olu�tu.
	if(hash[calculatedHash].key == key){   // �lgili indiste bulunan key de�eri ile kullan�c�n�n g�ndermi� oldu�u kelimenin key de�eri birbirine e�it mi ? 
		printf("\n%d adim gerceklesti.",adimSayisi);
		i=0;
	
		while(strlen(hash[calculatedHash].dosyaAdi[i]) > 0){     // ka� tane d�k�man ismi varsa say�s�n� tutuyoruz.
			i++;
		}
		int dosyaSayisi = i;
		for(i=0;i<dosyaSayisi;i++){  // d�k�man say�s� kadar d�ng� kurup kelimenin yan�na dosya isimlerini tek tek yaz�d�r�yoruz.
		printf("\n%s --> %s",hash[calculatedHash].word,hash[calculatedHash].dosyaAdi[i]);     
		}
	}
	else{
		printf("\n%d adim gerceklesti ve aranan kelime tabloda mevcut degil.",adimSayisi);
	}
}


int main(){
	char word[50];   // Aranacak kelimenin tutuldu�u dizi
	baslangicKeyDegeri();   // Hash tablosunda ba�lang��ta t�m KEY de�erlerini -1 olarak i�aretliyoruz.
	oncedenHashTablosuVarmi();   // Daha �nceden hash tablosu varm� onu kontrol ediyoruz.
	printf("\n Dokuman Eklemek icin '1' tusuna basin  /  Kelime Aramak icin '2' tusuna basin");
	int secim;
	scanf("%d",&secim);
	
	if(secim == 1){
		dokumanOku();   // // D�k�mandan gelen kelimeleri okuyup kelimeler i�in horner metoduna g�re key de�eri olu�turup bunu ekleHashTable fonksiyonuna yollay�p hashtable �zerinde gerekli indise yerle�tirmek �zere �a��rd���m�z fonksiyon
	}
	else if(secim == 2){
		printf("\nArayacaginiz kelimeyi giriniz : ");
		scanf("%s",word);
		kelimeAra(word);  // Kelime arama i�lemi
	}
	else{
		printf("Yanlis Tercih Yaptiniz !");
	}


	return 0;
}
