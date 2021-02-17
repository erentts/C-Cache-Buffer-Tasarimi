#include <stdio.h>
#include <stdlib.h>


double olasilik(double kazanmaOlasiligi,int yeterliGalibiyet){  // A tak�m�n�n t�m seriyi kazanma olas���n� d�nd�ren fonksiyon
	
	int i,j; // matris sat�r ve s�tun indislerini dola�abilmek i�in kullanaca��m�z de�i�kenler.
	double matris[yeterliGalibiyet+1][yeterliGalibiyet+1];  // Her iki tak�m�n geriye kalan ma�lar�n� tuttu�umuz i�in 0..4 aras� ma� say�s� kalabilir
	
	for(i=1;i<=yeterliGalibiyet;i++){
		matris[0][i] = 1;  // A tak�m�n�n kazanmas� i�in kalan ma�� 0 oldu�unda, B'nin kazanmas� gereken ma�lar var ise o zaman t�m seriyi A kazanm��t�r.
	}
	for(i=0;i<=yeterliGalibiyet;i++){
		matris[i][0] = 0;  	 // B tak�m�n�n kazanmas� i�in kalan ma�� 0 oldu�unda, A'n�n kazanmas� gereken ma�lar var ise o zaman t�m seriyi B kazanm��t�r.
	}
	
	for(i=1;i<=yeterliGalibiyet;i++){
		for(j=1;j<=yeterliGalibiyet;j++){
			// Mevcut indisimizin hemen �st�ndeki indisi kazanma olas�l��� ile �arp�p ve mevcut indisimizin hemen solundaki indis ile B tak�m�n�n kazanma olas���n� �arp�p bu iki de�eri topluyoruz.
			matris[i][j] = kazanmaOlasiligi * matris[i-1][j] + matris[i][j-1] * (1-kazanmaOlasiligi);  
		}
	}
	
	return matris[yeterliGalibiyet][yeterliGalibiyet];  // Matrisin en alt ve en sa� k��esindeki de�er olan A'n�n seriyi kazanma olas�l��� d�ner.
	
}


int main(){
	int macSayisi;  // Oynanacak ma� say�s�
	double kazanmaOlasiligi;   // A tak�m�n�n bir ma�� kazanma olas�l���
	int yeterliGalibiyet; 	// Bir tak�m�n t�m seriyi kazanmas� i�in gereken galibiyet say�s�
	printf("Oynanacak mac sayisini giriniz : ");
	scanf("%d",&macSayisi);
	printf("\nA takiminin bir maci kazanma olasiligini giriniz : ");
	scanf("%lf",&kazanmaOlasiligi);
	yeterliGalibiyet = macSayisi / 2 + 1;  // �rne�in 7 ma� girilince 7/2 = 3,5 olacak fakat int oldu�u i�in 3 yaz�lacak o y�zden 1 artt�r�yoruz.
	double sonuc = olasilik(kazanmaOlasiligi,yeterliGalibiyet);
	printf("\n7 maclik seride A'nin kazanma olasiligi : %.2lf \n",sonuc);
	return 0;
}
