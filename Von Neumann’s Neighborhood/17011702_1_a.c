#include <stdio.h>
#include <stdlib.h>

int *enYakinDegerlerBul(int dizi[],int n){  // Olusturdugumuz dizi ve onun eleman sayisini parametre olarak al�yoruz.
	if (n<2){         //  En yakin 2 nokta bulmam�z gerektigi icin minimum 2 elemanli bir dizi olmali !
		printf("\nEn yakin 2 deger icin bu dizi en az 2 degere sahip olmali !");
		return;
	}
	else {           // Kod, else blo�una girdi�inde en az 2 elemanl� bir diziye sahip oldu�umuzu anl�yoruz.
		int i;  // D�� d�ng� i�in saya� de�i�keni
		int j;  // �� d�ng� i�in saya� de�i�keni
		int enYakinFark = abs(dizi[0]-dizi[1]);  // Ba�lang�� olarak fonksiyona parametre olarak gelen dizinin ilk 2 eleman� aras�ndaki mutlak de�erce fark� enYakinFark de�i�kenine at�yoruz.
		int *enYakinDegerDizi = (int*)calloc(2,sizeof(int)); // Bulaca��m�z en yak�n 2 de�erin tutulaca�� dizi
		for (i=0;i<n-1;i++){    
			for(j=i+1;j<n;j++){
				if(abs(dizi[i] - dizi[j]) <= enYakinFark){   // Dizinin i. ve j(i+1). elemanlar�n�n mutlak de�erce fark� enYakinFark'tan k���kse;
					enYakinFark = abs(dizi[i] - dizi[j]);   // enYakinFark de�i�keni g�ncellenecek ve yeni indislerdeki say�lar�n mutlak de�erce fark� olacak.
					enYakinDegerDizi[0] = dizi[i];  // Ayr�ca, en yak�n iki de�eri enYakinDegerDizi dizimize at�p,
					enYakinDegerDizi[1] = dizi[j];  // dizimizdeki en yak�n iki de�eri g�ncellemi� olduk.
				}
			}
		}
		return enYakinDegerDizi;        // Son olarak en yak�n iki noktay� d�nd�rd�k.
	}
}

int main(){
	
	int n;     // Dizinin eleman say�s�
	int i;	   // Diziye eleman atmak i�in kullanaca��m�z saya� de�i�keni
	printf("Dizinin eleman sayisi giriniz: ");
	scanf("%d",&n);
	int dizi[n];       // Kullan�c�dan n elemanl� bir dizi olu�turmas�n� istedik.
	
	for(i=0;i<n;i++){
		printf("\n%d. elemani giriniz : ",i);
		scanf("%d",&dizi[i]);          // Dizimize de�erleri girdik.
	}
	
	printf("\nOlusturdugumuz dizi : ");
	for(i=0;i<n;i++){
		printf("\t%d",dizi[i]);        // Olu�turan dizimizi ekrana yazd�rd�k.
	}
	
	// ve son olarak kullan�c�ya en yak�n 2 noktay� belirttik.
	int *enYakinDegerDizi = enYakinDegerlerBul(dizi,n);
	printf("\n\nBirbirine en yakin iki eleman : %d ve %d",enYakinDegerDizi[0],enYakinDegerDizi[1]);   
	
	return 0;
}
