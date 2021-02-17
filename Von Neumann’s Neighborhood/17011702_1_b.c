#include <stdio.h>
#include <stdlib.h>


void birlestir(int dizi[],int firstIndex,int middleIndex,int lastIndex){  // mergeSort fonksiyonundan gelen dizi,firstIndex,middleIndex ve lastIndex parametreleri
    
	int firstSubArrayCount;  // �lk alt dizinin eleman say�s�
	firstSubArrayCount = middleIndex - firstIndex + 1;
    int secondSubArrayCount;	 // �kinci alt dizinin eleman say�s�
	secondSubArrayCount = lastIndex - middleIndex; 
    int i;  // D�ng� i�in saya� de�i�keni
	int firstSubArray[firstSubArrayCount];   // �lk alt dizimizi olu�turduk. --> firstSubArray [firstIndex...middleIndex]
	int secondSubArray[secondSubArrayCount]; // �kinci alt dizimizi olu�turduk. --> secondSubArray [firstIndex...middleIndex]
    
	for(i=0;i<firstSubArrayCount;i++){
		firstSubArray[i] = dizi[firstIndex+i];	 // Orta noktaya kadar olan de�erleri firstSubArray dizisine aktar�yoruz.
	}
        
    for(i=0;i<secondSubArrayCount;i++){
    	secondSubArray[i] = dizi[middleIndex+i+1];   // Orta nokta + 1 'den son elemana kadar olan de�erleri ise secondSubArray dizisine aktar�yoruz.
	}
    
	    
    int first;  // Birle�tirilmi� alt dizinin ba�lang�� indeksi.
	first = firstIndex;  
	int a = 0;  // �lk alt dizinin ba�lang�� indeksi.
	int b = 0;  // �kinci alt dizinin ba�lang�� indeksi.
       
    while(a<firstSubArrayCount && b<secondSubArrayCount){   // Her iki alt dizinin ba�lang�� index'inin kendi eleman say�lar�ndan k���k oldu�u sorguyu yapar�z.
        if(firstSubArray[a]<=secondSubArray[b]){   // E�er ilk alt dizinin ba�lang�� de�eri, ikinci alt dizinin ba�lang�� de�erinden k���k ise dizinin ilk eleman� olur.
            dizi[first]=firstSubArray[a];
            a++;
        }
        else{
            dizi[first]=secondSubArray[b];   // Aksi halde ise ikinci alt dizinin ilk eleman�, b�y�k dizinin ilk eleman� olur.
            b++;
        }
        first++;
    }
    
    while (a < firstSubArrayCount)   // E�er varsa kalan firstSubArray elemanlar�n� aktar�r�z.
    {
        dizi[first] = firstSubArray[a];
        a++;
        first++;
    }
 
    while (b < secondSubArrayCount)  // E�er varsa kalan secondSubArray elemanlar�n� aktar�r�z.
    {
        dizi[first] = secondSubArray[b];
        b++;
        first++;
    }
        
}

void mergeSort(int dizi[],int firstIndex,int lastIndex){ // Olu�turdu�umuz diziyi, o dizinin ilk ve son elemanlar�n� parametre olarak ald�k.
    if (lastIndex > firstIndex) 
    {
    	int middle; // Dizi i�in orta noktay� belirledi�imiz de�i�ken
        middle = (firstIndex+lastIndex)/2; //  Merge Sort'ta diziyi s�rekli ortadan ikiye ay�rd���m�z i�in her seferinde dizinin orta noktas�n� al�yoruz.
        mergeSort(dizi,firstIndex,middle); // �lk eleman ile orta noktaya kadar olan b�l�m� rekursif olarak kendi i�erisinde her defas�nda ortadan ikiye ay�r�p tek par�a haline getiriyoruz.
        mergeSort(dizi,middle+1,lastIndex); // Orta nokta + 1 ' den ba�layarak son elemana kadar olan b�l�m� rekursif olarak kendi i�erisinde her defas�nda ortadan ikiye ay�r�p tek par�a haline getiriyoruz.
        birlestir(dizi,firstIndex,middle,lastIndex);  // Tek par�a halinde kalanlar� birle�tir fonksiyonu ile yani merge mant���yla birle�iririz.
    }
    
}


int *enYakinDegerlerBul(int dizi[],int n){   // parametre olarak Merge Sort ile s�ralad���m�z diziyi ve dizinin eleman� say�s�n� yollad�k.
	int i;  // D�ng� saya� de�i�keni
	int enYakinFark;  // En yak�n iki de�er aras�ndaki mutlak de�erce fark� tutaca��m�z de�i�ken
	int *enYakinDegerDizi = (int*)calloc(2,sizeof(int));  // En yak�n iki de�eri tuttu�umuz dizi 
	enYakinFark = abs(dizi[0]-dizi[1]);  // Ba�lang�� olarak dizinin ilk ve ikinci indislerinin mutlak de�erce fark�n� ald�k. Bunu d�ng�de kullanaca��z.
	for(i=0;i<n;i++){
		if(enYakinFark >= (abs(dizi[i]-dizi[i+1]))){    // E�er bir �nceki en yak�n 2 de�erin mutlak de�erce fark� yeni sorgulad���m�z s�ral� iki de�erin mutlak de�erce fark�ndan b�y�k ise;
			enYakinFark = abs(dizi[i]-dizi[i+1]);   // enYakinFark de�i�kenimizi yeni gelecek iki de�erin mutlak de�erce fark�yla g�ncelle�tiriyoruz.
			enYakinDegerDizi[0] = dizi[i];  
			enYakinDegerDizi[1] = dizi[i+1];  // Ayr�ca, En yak�n iki noktay�da enYakinDegerDizi dizisine aktar�yoruz b�ylelikle enYakinDegerDizi dizisinide g�ncellemi� oluyoruz. 
		}
	}
	return enYakinDegerDizi;   // T�m i�lemler bittikten sonra en yak�n iki de�erin oldu�u enYakinDegerDizi dizisini d�nd�r�yoruz.
}


int main(){
	
	int n;  // Dizinin eleman say�s�
	int i;  // D�ng� i�in saya� de�i�keni
	printf("Dizinin eleman sayisi giriniz: ");
	scanf("%d",&n);
	int dizi[n];  // n elemanl� bir dizi olu�turduk.
	
	for(i=0;i<n;i++){
		printf("\n%d. elemani giriniz : ",i);
		scanf("%d",&dizi[i]);    // Kullan�c� diziye elemanlar� ekledi.
	}
	
	printf("\nOlusturdugumuz dizi : ");
	for(i=0;i<n;i++){
		printf("\t%d",dizi[i]);    // Olu�turdu�umuz diziyi ekrana yazd�rd�k.
	}
	
	mergeSort(dizi,0,n-1);   // Merge Sort ile diziyi s�ralad�k.
	
	printf("\n\nMerge Sort ile Siralanmis dizi : ");
	for(i=0;i<n;i++){
		printf("\t%d",dizi[i]);   // S�ralanm�� diziyi ekrana yazd�rd�k.
	}
	
	
	int *yeniDizi = enYakinDegerlerBul(dizi,n);
	printf("\n\nEn yakin iki nokta : %d ve %d",yeniDizi[0],yeniDizi[1]);   // En yak�n iki noktay� kullan�c�ya belirttik.
	
	
	return 0;
}
