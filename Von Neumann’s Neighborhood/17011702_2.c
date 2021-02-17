#include <stdio.h>
#include <stdlib.h>


int *VonNeumann(int **matris, int matrisBoyutu,int n){   // main'den gelen matris,matris boyutu ve kullan�c�dan al�nan n de�eri parametre olarak gelmi�tir.
	int *sayici; // Her sat�rda siyah renk olan karelerin say�s�n� dizimizin indislerine atabilmek i�in "sayici" isimli bir dizi olu�turuyoruz.
	int i; // D�ng� i�in sayac degiskeni
	int j; // D�ng� i�in saya� de�i�keni
	sayici = (int *)malloc(sizeof(int)*matrisBoyutu);   // her sat�r�n siyah kare say�s�n� atabilmek i�in bellekte yer a��yoruz.
	for (i=0;i<matrisBoyutu;i++){    
			sayici[i] = 0;		// Matris boyutu kadar i�erisini 0 de�eriyle dolduruyoruz bunun nedeni tuhaf de�erler gelmesini �nlemek.
	}
	for (i=0;i<matrisBoyutu;i++){	// {(x,y) : |x-x0| + |y-y0| <= n} ko�ulunu ger�ekledik.
		for (j=0;j<matrisBoyutu;j++){ // Bu kural do�rultusunda; matristeki siyah karelerin sat�r numaras� ile matrisin ortadaki eleman�n sat�r numaras�,
			if (abs(i-n-1)+abs(j-n-1)<=n){  // aras�ndaki fark�n mutlak de�eri ile s�tun numaralar� aras�ndaki fark�n mutlak de�erinin toplam� N de�erinden k���k veya e�it olmal�. 
				matris[i][j] = 1;   // Matrisin i. sat�r�n� ve j.s�tununu 1 olarak i�aretledik.
				sayici[i]++;   // ve ilgili sat�rda ka� tane 1(siyah kare) varsa bunlar�n toplam�n� ald�k.
			}
		}			
	}
	return sayici;   // Her sat�rdaki siyah kare say�s�n�n yer ald��� diziyi d�nd�r�yoruz.
}

int main() {

	int n;  // Kullan�c�dan al�nacak de�er 
	printf("N degerini giriniz : ");
	scanf("%d", &n);  // Kullan�c�dan al�nan de�er n de�i�kenine atan�yor.
	int matrisBoyutu;  // AxA ' l�k bir matris i�in n de�eri ile �ekildeki �r�nt� aras�nda ba� kuruyoruz.
	matrisBoyutu = 2*n+3;  // n = 0 i�in; 3x3 matris , n = 1 i�in; 5x5 matris , n = 2 i�in; 7x7 matris ...
	int i;  // d�ng� i�in kullan�lacak saya� de�i�keni
	int j;  // d�ng� i�in kullan�lacak saya� de�i�keni
	int **matris;  // �ok boyutlu matrisimizi olu�turuyoruz.
    int *sayici; //Her sat�rda siyah renk olan karelerin say�s�n� dizimizin indislerine atabilmek i�in "sayici" isimli bir dizi olu�turuyoruz.
	
	matris = (int **) malloc(sizeof(int*)*matrisBoyutu); // Her sat�r bellekte yer a��yoruz.
	if(matris == NULL){
		printf("Yetersiz bellek !");
	}
	
	// Her sat�r i�in gerekli olan s�tun say�s� i�in bellekte yer a��yoruz.
	for(i=0;i<matrisBoyutu;i++){
		matris[i] = (int *)malloc(sizeof(int)*matrisBoyutu );
		if(matris[i] == NULL){
			printf("Yetersiz bellek !");
		}
	}
	
	// Matris i�indeki t�m de�erleri 0 yap�yoruz. ��nk� biz matrisde gerekli alanlar� 1 yapaca��z ve kalanlar 0 olacak.	
	for (i=0;i<matrisBoyutu;i++){
		for (j=0;j<matrisBoyutu;j++){
			matris[i][j] = 0;
		}
	}	
	
	printf("\n");
	
	sayici = VonNeumann(matris,matrisBoyutu,n);   // Her sat�rdaki siyah kare say�s�n�n yer ald��� diziyi d�nd�rd�k ve sayac dizimize att�k.
		
	for (i=0;i<matrisBoyutu;i++){
		for (j=0;j<matrisBoyutu;j++){
			printf("%d ",matris[i][j]);   // AxA ' l�k matrisimizde 1(siyah kare) olan k�s�mlar� g�stermek i�in matrisi ekran yazd�rd�k.
		}
		printf("\n");
	}
	
	printf("\n");
	
	for (i=0;i<matrisBoyutu;i++){   // Her sat�rda ka� tane 1(siyah kare) varsa bunlar�n say�s� ekrana yazd�r�yoruz.
		printf("%d.Satir siyah kare sayisi : %d\n",i,sayici[i]);
	}
	
	// Ve matrisdeki toplam siyah karesini ekrana yazd�r�yoruz.
	printf("\nToplam siyah kare sayisi : %d",(2*n*(n+1))+1);
	
	// Burada matrisi haf�zadan silmemiz gerekti�i i�in sat�rlar� bo�alt�yoruz. 
	for(i=0;i<matrisBoyutu;i++) {
		free(matris[i]);
	}
	
	// Sat�rlar� bo�alan matrisi bo� olarak ifade ediyoruz.
	free(matris);
	
	return 0; 	
}

