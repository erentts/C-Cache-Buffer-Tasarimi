#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct link{
	char harf;
	int frequency;
	struct link *next,*left,*right;
}node;

node *head = NULL;

node *Olustur(char harf){  // Her yeni dugumu olusturdugum fonksiyon.
	node *yenidugum = (node*)malloc(sizeof(node));
	yenidugum->frequency = 1; 
	yenidugum->harf = harf;
	yenidugum->next = NULL;
	yenidugum->left = NULL;
	yenidugum->right = NULL;
	return yenidugum;
}
void SonaEkle(char harf){ 
	
	node *sonaeklenecek = Olustur(harf);  //Olustur() fonksiyonumuzdan gelen bilgileri yeni bir node'a atad�m.
	if (head == NULL){  // E�er linkli listemiz bo� ise ilk de�er atamas� yap�lacak.
		head = sonaeklenecek;
		return;
	}
	int kontrol=0; // kontrol degiskenimizi e�er yeni gelen harfin daha �nce listede olup olmad���n� sorgulamak i�in kulland�k.
	node *current = head;
	while(current != NULL){
		if (current->harf == harf){ // Gelen yeni harfin linkli listede olup olmad���n� sorguluyoruz.
			current->frequency++; // E�er gelen harf listemizde mevcutsa frekans�n� artt�r�yoruz.
			kontrol++; 
			return;
		}
		current = current->next;
	}
	if (kontrol == 0){  // kontrol-> 0 ise; gelen yeni harf listemizde yoktu ve yeni d���m olu�turduk.
		node *temp = head;
		while(temp->next != NULL){
			temp = temp->next;
		}
		temp->next = sonaeklenecek;
	}
	else{
		head = current; // kontrol-> 1 olmas� halinde yukar�daki frekans artt�rma i�lemi yap�ld� ve head'e at�ld�.
	}
}

node *insertion_sort(node* head2)   // Insertion sort i�lemleri..
{
    node *temp1, *temp2, *temp3;
    temp1 = head2;
    head2 = NULL;
 
    while(temp1 != NULL)
    {
        temp3 = temp1;
        temp1 = temp1->next;
        if (head2 != NULL)
        {
            if(temp3->frequency > head2->frequency)
            {
                temp2 = head2;
                while ((temp2->next != NULL) && (temp3->frequency> temp2->next->frequency))
                {
                    temp2 = temp2->next;
                }
                temp3->next = temp2->next;
                temp2->next = temp3;
            }
            else
            {
                temp3->next = head2;
                head2 = temp3;
            }
        }
        else
        {
            temp3->next = NULL;
            head2 = temp3;
        }
    }
    return head2;
}

void huffman(){
	
	int sayici=0;
	node *sayac = head;
	while(sayac != NULL){ // huffman tree'deki en tepeki i�lem yapt���m�z elemanlar�n say�s�n� al�yorum.
		sayici++; // bu sayiyi almam�n nedeni tepede yaln�zca 1 say� kald���nda i�lemi sonland�rmak.
		sayac = sayac->next;
	}
	while(sayici!=1){ // sayici de�i�keni her i�lem yap�ld�g�nda 1 azalt�l�yor.E�er sayici = 1 ise i�lemimiz bitti ve huffman a�ac� olu�tu demektir.
		node *tut = head; // �ncelikle mevcut head'imizi tut isimli yeni bir node'a aktar�yoruz.
		node *temp = (node*)malloc(sizeof(node));
		temp->left = NULL;
		temp->right = NULL;
		temp->next=NULL;
		temp->frequency = tut->frequency+tut->next->frequency; // temp node'u i�in frekans de�eri atamas�.
		temp->harf = ' '; // harf bo� olaca�� i�in bo� karakter at�yoruz.
		temp->left = tut;
		temp->right = tut->next;
		head = tut->next->next;


		node *current;
		current = temp; // olu�turdugumuz temp'i head'imize ekleme i�lemleri ve ekledikten sonra insertion_sort fonksiyonumuzla s�ralamay� g�ncelliyoruz.
		current->next = head;
		head = current;
		head = insertion_sort(head);
		
		sayici--; 
	}
}

void yazdir(){ // Liste yazdirma fonksiyonu
	node *temp = head;
	while(temp != NULL){
		printf("%d%c  ",temp->frequency,temp->harf);
		temp = temp->next;
	}
}

void yazdir_level(node* tree, int level) // A�a�taki her seviyeyi ayr� ayr� olarak main fonksiyonumuza �ekip orada for ile t�m seviyeleri yazd�r�yoruz.
{ 
    if (tree == NULL){
    	return;
	}  
    if (level == 1){
    	printf("%d%c  ",tree->frequency,tree->harf); 
	}
    else if (level > 1) 
    { 
        yazdir_level(tree->left,level-1);  // level-1 olarak almam�z�n sebebi; �rne�in:level=3 olsun; 
        yazdir_level(tree->right,level-1); // 3 kere left veya right i�lemleri ile 3.seviyeye inmemiz gerekiyor.Bu nedenle;
      								   	   // level'i recursion i�lemi ile 3 den 1 e getiriyoruz b�ylikle,
      								   	   // right ve left i�lemleri ile 3.seviyeye inmi� oluyoruz.
    } 
} 

int tree_height(node *tree) // a�a� y�ksekli�ini bulmam�z gerekiyor yazdirma islemi icin.
{ 
    if (tree==NULL){
    	return 0;
	}  
    else{ 
		int left_height = tree_height(tree->left); 
        int right_height = tree_height(tree->right);
        
        if (left_height > right_height){ // Burada en uzun hangisi onu sorgulad�k ��nk� bize en uzunu laz�m.
        	return (left_height+1);
		} 
        else {
        	return (right_height+1);	
		} 
    } 
} 

int main(){
	char text[200];
	printf("\n Bir yazi giriniz : ");
	gets(text);
	printf("\n");
	int i=0;
	while(text[i] != '\0'){	
		SonaEkle(text[i]); 
		i++;
	}
	yazdir();
	printf("\n\n ******Siralanmis liste ****\n\n");
	head = insertion_sort(head);
	yazdir();
	
	printf("\n\n ****** HUFFMAN TREE ****\n\n");
	huffman();
	
	int a,b,treeheight;
	treeheight = tree_height(head); //a�ac�n y�ksekligini treeheight degiskenine atad�k ve i=1 den ba�lay�p treeheight'a kadar levelleri yazd�racag�z.
	for (a=1;a<=treeheight;a++){
		yazdir_level(head,a);
		printf("\n");
	}	
	
	return 0;
}
	

