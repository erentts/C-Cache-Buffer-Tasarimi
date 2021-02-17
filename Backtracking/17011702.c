#include <stdio.h>
#include <stdlib.h>

int kontrol(int satir,int sutun,int matrisBoyut,int **matris){   // S�tunlarda ayn� renk varm� bunu kontrol etti�imiz fonksiyon
	int i;
	for(i=0;i<matrisBoyut;i++){
		if((matris[i][sutun] == matris[satir][sutun]) && (i != satir) && i<satir){
			// Gelen sat�r ve s�tun de�erine g�re matrisin o indisindeki de�eri,
			// ayn� s�tunda ve �st�nde yer alan di�er renkler ile kar��la�t�r�l�r.
			// �artlar sa�lan�rsa ayn� renk vard�r ve 1 de�eri d�nd�r�r�z.  
			return 1;
		}
	}
	return 0; // Aksi halde 0 d�ner.
}

void matrisYazdir(int **matris,int matrisBoyut){   // Matris Yazdirma Fonksiyonu
	int i,j;  // Matriste gezinmek i�in de�i�kenler
	for(i=0;i<matrisBoyut;i++){
		for(j=0;j<matrisBoyut;j++){
			if(matris[i][j] == 1){
				printf("Kirmizi (%d) \t",matris[i][j]);    // 1 - Kirmizi
			}
			else if(matris[i][j] == 2){
				printf("Sari    (%d) \t",matris[i][j]);    //  2 - Sari
			}
			else if(matris[i][j] == 3){
				printf("Beyaz   (%d) \t",matris[i][j]);	   //  3 - Beyaz
			}
			else if(matris[i][j] == 4){
				printf("Mavi    (%d) \t",matris[i][j]);    //  4 - Mavi
			}
			else if(matris[i][j] == 5){
				printf("Mor     (%d) \t",matris[i][j]);    //  5 - Mor
			}
			else if(matris[i][j] == 6){
				printf("Turuncu (%d) \t",matris[i][j]);    //  6 - Turuncu
			}
			else if(matris[i][j] == 7){
				printf("Yesil   (%d) \t",matris[i][j]);    //  7 - Yesil
			}
			else if(matris[i][j] == 8){
				printf("Siyah   (%d) \t",matris[i][j]);    //  8 - Siyah
			}	
		}
		printf("\n");
	}
	printf("\n\n");
}

void sagaKaydir(int **matris,int matrisBoyut,int satir){  // Sa�a kayd�rma i�leminin yap�ld��� fonksiyon
	int i, temp;
	temp = matris[satir][matrisBoyut-1]; // Sa�a kayd�ralacak olan sat�rda son s�tun de�erini temp de�i�keninde ge�ici olarak tutuyoruz.
	i = matrisBoyut-1;  // Son indis hari� di�erlerini sa�a kayd�rabilmek i�in d�ng� say�m�z.
	while (i > 0){
		matris[satir][i] = matris[satir][i-1]; // Son eleman� direk ilk s�tuna koyaca��z bu y�zden onu ge�ici de�i�kende tutup di�erlerini bir indis sa�a kayd�rd�k.
		i--;
	}
	matris[satir][0] = temp;  // temp de�i�keninde tuttu�umuz son indiside ilk s�tuna koyduk.
}

int backtracking(int matrisBoyut,int **matris,int sabitMatrisBoyut){   // Backtracking Fonksiyonu
	int i,j;
	int sayac=0;
	
	for(i=0;i<matrisBoyut;i++){
		for(j=0;j<matrisBoyut;j++){   // Matrisi geziyoruz.
			if(kontrol(i,j,matrisBoyut,matris) == 1){   // E�er kontrol == 1 ise o zaman s�tunlara ayn� de�er yerle�mi�tir.
				sagaKaydir(matris,sabitMatrisBoyut,i);  // Sa�a kayd�rma i�lemi yap�yoruz.
				if(kontrol(i,j,matrisBoyut,matris) != 1){  // Sa�a kayd�rma i�lemi bitti�inde ad�m ad�m yazd�r�yoruz.
					printf("*********** SAGA KAYDIRILDI ***********\n",i);
					matrisYazdir(matris,sabitMatrisBoyut);
				}
			}
			else {
				backtracking(matrisBoyut-1,matris,sabitMatrisBoyut);   // Rekursif olarak geri izlemeye devam edilir.
			}
		}
	}
}


int main(){
	int i,j;  // Matriste gezinmek i�in de�i�kenler
	int **matris;  // Renk Matrisi
	int n;  // Matris i�in boyut
	printf("N x N lik bir matris icin n giriniz : ");
	scanf("%d",&n);
	if(n<3 || n>8){    // 3 ile 8 aras�nda matris boyut kontrol�
		printf("Lutfen 3 ile 8 arasinda bir matris boyutu giriniz !");
		return 0;
	}	
	
	matris = (int**)calloc(sizeof(int*),n);  // Matris Allocate Sat�r ve S�tunlar i�in
	for(i=0;i<n;i++){
		matris[i] = (int*)calloc(sizeof(int),n); 
	}
	printf("\n1-Kirmizi\n2-Sari\n3-Beyaz\n4-Mavi\n5-Mor\n6-Turuncu\n7-Yesil\n8-Siyah\n");
	printf("\nRenk matrisini yukarida verilen her renk icin sayi karsiliklarini giriniz : \n\n");
	for(i=0;i<n;i++){
		printf("%d. satirin renklerini giriniz\n",i+1);
		for(j=0;j<n;j++){
			scanf("%d",&matris[i][j]);    // Matrisin indislerine renkleri say�sal olarak yerle�tiriyoruz.
		}
		printf("\n");
	}
	
	printf("***** Matrisin ilk hali *******\n");
	matrisYazdir(matris,n);
	backtracking(n,matris,n);   // Backtracking i�lemleri i�in fonksiyonu �a��r�yoruz.
	
	
	
	int test = 0;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){    
			if(kontrol(i,j,n,matris) == 1){   // Matrisde ayn� s�tunlarda ayn� renkler yerle�mi� mi son olarak kontrol ediyoruz.
				test = 1;
			}
		}
	}
	
	if(test == 1){
		printf("\n\nBU MATRIS ICIN SONUC URETILEMEDI !!!!!!!!!!\n\n");
	}
	
	printf("***** Matrisin son hali *******\n");
	matrisYazdir(matris,n);
	
	
	
	
	

	for( i = 0; i < n; i++ ) {   // Matris ile i�imiz bitince sat�rlar�n i�ini bo�alt�yoruz.
		free( matris[i] );
	}
	free( matris );    // Sat�rlar silindikten sonra en son matrisi bo� olarak i�aretliyoruz.

	
	return 0;
}
