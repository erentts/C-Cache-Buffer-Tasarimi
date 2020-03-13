#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct link{
	char adres[30];
	int sayac;
	struct link *next; // Doubly Linked List yap�s�na uyarlamak i�in ->next ve ->prev olu�turuyorum.
	struct link *prev;
}node;

node *head = NULL;

node *olustur(char adres[30]){  // Her d���m� node *olustur fonksiyonu icinde olu�turup BasaEkle() fonksiyonuna yolluyorum..
	node *yenidugum = (node *) malloc(sizeof(node));  // yollanan char icin bellekte yer ac�yorum..
	yenidugum->sayac = 1;  // ilk defa olusturuldugu icin sayac=1 olarak ba�lat�yorum..
	strcpy(yenidugum->adres,adres);  
	yenidugum->next = NULL; // Her deger ald�g�mda ilk deger gibi ald�g�mdan dolay� ->next = NULL ve ->prev = NULL atamas� yap�yorum.
	yenidugum->prev = NULL;
	return yenidugum;
}

void BasaEkle(char adres[30],int kapasite,int esik){
	node *basaekle = olustur(adres);
	if(head==NULL) // head nodu NULL ise bo� demektir o zaman node *olustur() fonksiyonundan gelen bilgiyi head noduna ekliyorum.
	{
		head = basaekle;
		return;
	}
	
	int kontrol=0; // kontrol de�i�keni koyma nedenim dosyadan gelen string bilginin linked list icinde olup olmad���n� sorgulamak icin kullanaca��m.
	node *current = head;
	while(current!=NULL)
	{
		if (strcmp(current->adres,adres) == 0) // Dosyadan gelen deger linked list icinde varm� ? e�er var ise if blo�una giri� yapaca��z.
		{
			if (current->sayac <esik || current->sayac >esik) // Gelen string'in sahip oldugu sayac e�ik de�erden d���k veya e�ik de�erden b�y�kse sadece sayac�nda artt�rma olacak.
			{
				current->sayac++;
				kontrol++;
				return;
			}
			else // else durumunda ise geriye tek ko�ul kal�yo oda string'in gelen adres'e e�it olmas�.
			{
				
						if (current->prev == NULL){ // �nceki de�er NULL ise o en ba�taki elemanday�z bu nedenle sayac�n� sadece artt�rmam�z yeterlidir.
							current->sayac++;
							return;
						}
						if (current->next == NULL)  // Sondaki degerin next'i NULL ise o zaman sondaki elemanday�z ve bunu ba�a alaca��z...
						{
							current->sayac++;
							node *onceki_kisim = current->prev;
							onceki_kisim->next = NULL;
							node *item2 = (node*)malloc(sizeof(node));
							strcpy(item2->adres,current->adres);
							item2->sayac = current->sayac;
							item2->prev = NULL;
							free(current);
							node *eski = head;
							head = item2;
							head->next = eski;
							eski->prev = head;	
							return;
						}
						
						
						// �nceki iki if komutunda ilk ba�taki node yada son durumdaki node'un olup olmad�g�n� sorgulad�k ve ba�ta ise zaten ba�ta kalacak sonda ise ba�a alacakt�k.
						//E�er node ba�ta ve sonda de�ilse tek se�enek geriye kal�yor oda node'un ortada olmas�.
						current->sayac++;
						node *onceki = current->prev;
						node *sonraki = current->next;
						onceki->next = sonraki;
						sonraki->prev = onceki;
						node *item = (node*)malloc(sizeof(node));
						strcpy(item->adres,current->adres);
						item->sayac = current->sayac;
						item->prev = NULL;
						free(current);
						node *eski = head;
						head = item;
						head->next = eski;
						eski->prev = head;
						return;
					}
					
				}
					current = current->next; 
			}
	

	if (kontrol == 0) // kontrol degiskeninin hala s�f�r olmas� demek yani linked list icinde dosyadan gelen string ile e�le�en bir veri bulamad��� anlam�na geliyor.
	{
		// KAPAS�TE KONTROL�
		node *say = head;
		int sayici=0;
		while(say->next != NULL) // Listemizde ne kadar kutu var onu ��renmek icin bir sayici degiskeni kulland�k bunu kapasite kontrol�nde kullanaca��z.
		{
			say = say->next;
			sayici++;
		}
		if (sayici == kapasite){  // Listemizdeki kutular say�ld�ktan sonra art�k kullan�c�n�n girdi�i kapasite ile e�it olup olmad���n� sorgulayabiliriz.
			node *tempp = head;
			while(tempp->next != NULL)
			{
				tempp = tempp->next;
			}
			node *oncekii = tempp->prev;
			oncekii->next = NULL;
			free(tempp);
			
			
			node *eski = head;
			head = basaekle;
			head->next = eski;
			eski->prev = head;
		}
		else { 
			node *eski = head;
			head = basaekle;
			head->next = eski;
			eski->prev = head;
		}
	}
	else
	{
		head = current;	
	}
	
}

void yazdir(node *baslangic) // Ekrana yazma fonksiyonumuz..
{
	node *temp = baslangic;
	printf("\n");
	while(temp!=NULL)
	{
		printf("%s,%d\t",temp->adres,temp->sayac);
		temp = temp->next;	
	}
}

int main(){
	char adres[30];
	int secim,secim2,n,i=0;
	int kapasite,esik;
	printf("Kapasite kac olsun : ");
	scanf("%d",&kapasite);
	printf("Esik deger kac olsun : ");
	scanf("%d",&esik);
	printf("Klavyeden Okumak icin ' 1 ' tusuna basiniz.. \nDosyadan Okumak icin ' 2 ' tusuna basiniz.  ");
	scanf("%d",&secim2);
	int sayac=0;
	FILE *fp; 
	char buff[255];
	switch(secim2){   // Burada switch kullanmam�n sebebi; dosyadan m� adresleri �ekece�iz yoksa klavyeden adres mi girece�iz tercihini kullan�c�ya yapt�rmak i�in.
		case 1: // CASE 1: Klavyeden girilecek.
			printf("Kac string gireceksiniz : ");
			scanf("%d",&n);
			while(i<n){
			printf("\n Eklemek istediginiz metni girin .. ");
			scanf("%s",&adres);
			BasaEkle(adres,kapasite-1,esik);
			yazdir(head);
			printf("\n");
			i++;
	}
		break;
		
		case 2: // CASE 2: Dosyadan okunacak.
   				fp = fopen("input.txt", "r");  
   				while(fscanf(fp, "%s", buff)!=EOF){  
   				strcpy(adres,buff);
				BasaEkle(adres,kapasite-1,esik);
				yazdir(head); 
				sayac++;
   				}
   				fclose(fp);  
				break;
		default: break;
	
	}
	printf("\n\n Cache Buffer'i Temizlemek istiyormusunuz ?\n \t(1-EVET)\t(2-HAYIR) \n\n");
	scanf("%d",&secim);
	switch (secim) // Kullan�c�n�n Cache Buffer'i silebilmesine olanak tan�mak i�in yeniden bir se�im yapt�rd�m.
	{              
		case 1: printf("\n\n ******SILINIYOR*****\n\n");
				int j;
				for (j=0;j<kapasite;j++)
				{
					if (head==NULL){
						return;
					}
					if (head->next == NULL)
					{
						head = NULL;
						printf("\n Silme Islemi TAMAMLANDI..");
						return;
					}
					node *tut = head->next;
					tut->prev = NULL;
					free(head);
					head = tut;
					yazdir(head);
					printf("\t\tSilindi !\n");
				}
				break;
		case 2: 
				printf("Silmemeyi tercih ettiniz.Listenin son hali\n");
				yazdir(head);
				break;
		default: break;
	}
	return 0;
}
