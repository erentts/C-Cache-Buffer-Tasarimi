#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define BOYUT 25000   // �ok uzun bir metinde girilebilece�inden dolay� rastgele �ok y�ksek verdim.



void tablo_olustur(char aranankelime[50], int aranan_uzunluk, int tablo[BOYUT], int kontrol) // Badmatch tablosu olu�turuyoruz..
{
	int i;
	for(i=0;i<BOYUT;++i)
	{
		tablo[i] = aranan_uzunluk;  // �ncelikle arad���m�z string'in uzunlugunu tablomuzun indislerine atad�k.
	}
	for(i=0;i<aranan_uzunluk-1;++i){
		
		if (kontrol != 0){  // kontrol degiskenini almam�n nedeni not case sensitive yada case sensitive kullan�l�p kullan�lmayacag�n� sorgulamak icin.
			if (aranankelime[i]>'a'){
				tablo[aranankelime[i]+'B'-'b'] = aranan_uzunluk-i-1;	// Not Case sensitive durumunda;
			}                                   // arad���m�z stringdeki karakterlerin kayma miktarlar�n� belirledik.
			else if(aranankelime[i]<='a'){               
				tablo[aranankelime[i]-'B'+'b'] = aranan_uzunluk-i-1;  
			}		
		}	
		tablo[aranankelime[i]] = aranan_uzunluk-i-1;  // Arad���m�z stringdeki karakterlerin kayma miktarlar�n� belirledik.
	}

}


int en_buyuk (int x,int y) {  // En b�y�k deger fonksiyonu
	if (x>y){
		return x;
	}
	else{
		return y;
	}
}


// Boyer Moore Horspool
int boyer_moore_horspool(char aranankelime[50], int aranan_uzunluk, char metin[BOYUT], int metin_uzunluk,char yenikelime[50],int yeni_uzunluk,int kontrol)  
{
	int i,a,k,tablo[BOYUT];
	tablo_olustur(aranankelime, aranan_uzunluk, tablo, kontrol); 
	
	// String icinde arama islemleri
	int islem_sayisi=0;  // found and replace i�lemlerinin ka� adet yap�ld�g�n� bu degisken icinde tutuyorum.
	int j = 0;
	while (j <= (metin_uzunluk - aranan_uzunluk)) {
		int k = aranan_uzunluk-1;
        while(((aranankelime[k] == metin[j+k]) && (k >= 0) || (kontrol == 1 && (fabs(aranankelime[k]-metin[j+k])==fabs('A'-'a'))))){  
			k--;  // case sensitive kontrol� ve aranan kelimenin metin i�inde e�le�ip e�le�meme durumunu sorguluyoruz.
		} // e�le�tirme yaparken tek tek karakterlere bak�l�yor ve k de�eri azalt�l�yor e�er k<0 olursa t�m karakterlerimiz e�le�ti ve;
        // kelime bulundu anlam�na geliyor.
        if (k < 0)
        {
				// REPLACE �SLEMLER�
				if (aranan_uzunluk==yeni_uzunluk){ // E�er arad���m�z kelimenin karakter say�s� , yeni girdi�imiz kelimenin karakter say�s� ile e�itse;
					for(i=0;i<aranan_uzunluk;i++){ // shift yapmadan direk karakterlerin yerlerini de�i�tiriyorum.
						metin[j] = yenikelime[i];
						j++;
					}	
				}
				else if (yeni_uzunluk>aranan_uzunluk){  // E�er yeni kelimenin karakter say�s� , arad���m�z kelimenin karakter say�s�ndan b�y�k ise;	
					for (i=0;i<(yeni_uzunluk-aranan_uzunluk);i++){ ; // Sa�a shift yapmam gerekiyor ve bu shift miktar� (yeni_kelime - aranan_kelime) kadar olacak
						for(a=metin_uzunluk;a>j;a--){ // �rnek : 70.indis son indisim ise, 1 kere sa�a shift yapacaksam art�k son indisim 71.indis olacak.
							metin[a+1] = metin[a];
							
						}
					metin_uzunluk = metin_uzunluk + (yeni_uzunluk-aranan_uzunluk);
					}
					for(i=0;i<yeni_uzunluk;i++){ // Yukar�da yapt���m sa�a shift i�leminden sonra art�k kelimeleri eklemek kald� ve onlar�da eklemi� oldum.
						metin[j] = yenikelime[i];
						j++;
					}
				}
				else {    // Buras� son durum.Yani aranan kelimenin karakter say�s�n�n, yeni gelen kelimenin karakter say�s�ndan b�y�k olmas� durumunda;
					for (i=0;i<(aranan_uzunluk-yeni_uzunluk);i++){ // Sola shift i�lemi yapmam�z gerekiyor ve bu shift miktar� (aranan_kelime - yeni_kelime) kadar olacak.
						for(a=(j+yeni_uzunluk);a<metin_uzunluk;a++){ // �rne�in ilk e�le�en kelimenin indisi 21 olsun; ilk 20 karakteri sabit tutup kelimenin sa��nda kalan
							metin[a] = metin[a+1]; // karakterleri shift miktar� kadar sola kayd�r�yorum.
							
						}
						
					}
					for(i=0;i<yeni_uzunluk;i++){ // shift islemi bittikten sonra kelimeleri ekliyorum.
						metin[j] = yenikelime[i];
						j++;
					}
				
				}

			if ((j+aranan_uzunluk)<metin_uzunluk){      	
            	j = j + aranan_uzunluk-tablo[metin[j+aranan_uzunluk]];
			}
			else{
				j = j+1;
			}
			islem_sayisi++; // Found and replace i�lemi ka� kez oldugunu kontrol etti�im de�i�ken.  
        }
        else{
        	j = j + en_buyuk(1,tablo[metin[j+aranan_uzunluk-1]]);
		}  
		
    }
    return islem_sayisi;
}

int main(){
	struct timespec baslangic;
	struct timespec bitis;
	char metin[BOYUT];
	char aranankelime[50];
	char yenikelime[50];
	int kontrol;
	
	int islem_sayisi;
	
	printf("Aramak istediginiz kelimeyi giriniz : ");
	gets(aranankelime);
	printf("\nYeni kelimeyi giriniz : ");
	gets(yenikelime);
	
	char dosya_isim[50];
	printf("Dosya ismi girin (Lutfen .txt uzantisi eklemeyi unutmayin !) : ");
	scanf( "%s",dosya_isim);
	
	FILE *fp;
	if ((fp = fopen(dosya_isim, "r")) == NULL) {
	    printf("Dosya acilmadi..");
	    exit(1);
	}
	
	printf("\n\nDosya icindeki metin : \n\n");  // Dosyadaki metni dizimize aktar�p ayn� zamanda console'da kullan�c� ekran�na ��kmas�n� sa�lad�k.
	fread(metin,sizeof(char),BOYUT,fp);
	
	fclose(fp);
	int count=0;
	while (metin[count] != '\0'){
		printf("%c",metin[count]);
		count++;
	}
	
	printf("\n\nTercih : \n\nNot Case Sensitive icin 1'e basin. \nCase Sensitive icin 0'a basin. \n");
	scanf("%d",&kontrol);
	
	printf("\nAranan Kelime : %s \t Yeni Kelime : %s\n",aranankelime,yenikelime);
	
	clock_gettime(CLOCK_MONOTONIC, &baslangic); // Arama ve yerde�i�tirme i�lemlerinin ne kadar s�rede ger�ekle�ti�ini  g�stermek i�in kulland�k.
	islem_sayisi = boyer_moore_horspool(aranankelime, strlen(aranankelime),metin, strlen(metin),yenikelime,strlen(yenikelime),kontrol); // found and replace say�s�
	clock_gettime(CLOCK_MONOTONIC, &bitis);
	
	FILE *fp2 = fopen(dosya_isim,"w");
	int sayac=0;
	while (metin[sayac]!='\0'){  // Found and Replace i�lemlerinin ard�ndan gelen yeni text'i dosyaya yaz�yoruz.
		fputc(metin[sayac],fp2);
		sayac++;
	}

	fclose(fp2);

	
	printf("\n\nYeni metin : \n\n%s\n\nFound and Replace : %d",metin,islem_sayisi);
	
	
	printf( "\n\nIslem suresi : %.9lf milisaniye \n\n", (double)((long int)(bitis.tv_sec-baslangic.tv_sec)*1000000000 + (bitis.tv_nsec-baslangic.tv_nsec)) / 1000000 );
	return 0;
}
	
   		
	
	
	

