#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include<unistd.h>
#include<string.h>
#include<fstream>
#include "console.h"
using namespace std;
/*============================================*/
//Khai bao cac bien toan cuc
const int maxpile = 10;		// so dong lon nhat co the tao ra
const int maxstone = 30;             //so soi lon nhat trong 1 dong
int status;			        //luot di, =0->may di||A di, =1->nguoi di||B di
int piles_at_fisrt, piles_at_pre; //so dong ban dau va so dong hien tai
bool test=false; 
/*==============================================*/
//Khai bao cau truc du lieu
struct stone_pile {
   int		stone;                     // stone = soi
   stone_pile *next;
};
typedef stone_pile *list;
list root = NULL;
/*=============================================*/
void put_piles(int &m)          // ham nhap vao so dong soi
{
    do {
   	printf("\n Nhap so dong soi muon choi: ");       
            scanf("%d", &m);
          } while (m<=0 || m > maxpile);
}
/*============================================*/
void title(int i)
{
   clrscr();
   gotoxy(34,2);
   printf("NIM GAME",13);
   gotoxy(43,2);
   switch (i) {
   			case 1:
      	             printf("- MISERE"); break;
           case 2:
      	             printf("- NORMAL"); break;
           
          default: break;
   }
}
/*==============================================*/
void createsolution(list &root,int n)
{
	list cur, temp;
	int i,a;
   	temp = new stone_pile; 
	printf("nhap so soi dong 1: " );
    scanf("%d",&a);  
	temp->stone=a;    		//tao dong dau tien
   	temp->next = NULL;
   	root = temp;
   	cur = root;
   	srand(time(NULL));
   	for (i=1; i<n; i++)        	//tao cac dong tiep theo
   	{
      		temp = new stone_pile;
      		printf("nhap so soi dong %d: ",i+1);
      		scanf("%d",&a);
      		temp->stone=a;		
      		temp->next = NULL;
      		cur->next = temp;
      		cur = temp;
   	}	
}
/*=============================================*/
void create(list &root, int n)		//tao ra n dong soi voi so soi ngau nhien
{
list cur, temp;
	int i;
   	temp = new stone_pile;       		//tao dong dau tien
   	temp->stone = rand()%maxstone+1;  //de tranh tao ra dong co 0 vien soi
   	temp->next = NULL;
   	root = temp;
   	cur = root;
   	srand(time(NULL));
   	for (i=1; i<n; i++)        	//tao cac dong tiep theo
   	{
      		temp = new stone_pile;
      		temp->stone = rand()%maxstone+1;
      		temp->next = NULL;
      		cur->next = temp;
      		cur = temp;
   	}
}
void display_table(list root)
{
   	int i = 1, x1 = 5, x2 = 13, y = 6 ;
   	list cur;
   	cur = root;   
   	printf("\n"); printf(" +------+--------+");
   	printf("\n"); printf(" | Dong | So soi |");
   	printf("\n"); printf(" +------+--------+");
   	while (cur != NULL)
   	{
      		gotoxy(1, y);
      		printf("|      |        |          |"); printf("\n");
      		printf(" +------+--------+          |");
      		gotoxy(x1, y); 
			printf("%2d", i);
      		gotoxy(x2, y); 
			printf("%2d", cur->stone);
      		i++;  y +=2;
      		cur = cur->next;
   	}
}
/*===========================================*/
int check_stone(list root, int k)  //kiem tra dong muon boc co hop le ko
{
int i;
list tmp = root;
for(i=1; i<k; i++) tmp = tmp->next;
if (tmp->stone) return 1;
else return 0;
}
/*=============================================*/
void player(list &root)
{
   int  k, n, i;
   list tmp = root;
   do
   {
   	gotoxy(30,7);
   	printf(" Moi ban chon dong :              ");
   	gotoxy(51,7);
   	scanf("%d",&k);
   } while (k<=0 || k > piles_at_fisrt || check_stone(root, k)==0);

   for(i=1;i<k; i++) tmp = tmp->next;
   do 
   {
      	gotoxy(30,9);
      	printf(" Va so soi muon boc:              ");
      	gotoxy(51,9);
      	scanf("%d", &n);
   } while (n<=0 || n > tmp->stone);
   tmp->stone = tmp->stone - n;
   if (tmp->stone ==0) piles_at_pre--;  //neu boc het thi giam so dong soi
}
/*=============================================*/
int nimsum(list root)
{
   int s;
   list cur = root;
   s = cur->stone;
   while (cur->next !=NULL) 
  {
      	cur = cur->next;
      	s = s ^ cur->stone;
   }
   return s;
}
/*=============================================*/
int special_case(list root)  //kiem tra truong hop dac biet
//neu dung tra ve vi tri dong co soi >1 ; neu sai tra ve 0
{
	int i=0, i1=0;    //bien dem so dong 1
	int pos;     // bien chi vi tri dong soi hien tai
	list cur;
	cur = root;
	while (cur!=NULL)
	{
		if(cur->stone ==1) i1++;
		else if (cur->stone >1) pos = i+1;
		cur= cur->next;
        i++;
	 }
	if (i1== (piles_at_pre-1)) return pos;
	else return 0;
}
/*=============================================*/
int pile_max(list root, list &max) //tim ra dong co so soi lon nhat
{
   list  cur;
   int imax, i;
   max = root;
   imax=1;            //vi tri cua dong max
   cur = root->next;
   i = 2;
   while(cur !=NULL)
   {
      	if (cur->stone > max->stone) { max = cur; imax =i;}
      	cur = cur->next;
      	i ++;
   }
   return imax;
}
/*=============================================*/
void misere_stragety(list &root)          //chien thuat misere cua may'
{
   list tmp = root;
   int take, s, i;
   i=special_case(root);         //vi tri cua dong co nhieu vien soi
   if (i) {    				//roi vao truong hop dac biet
		for(int j=1; j< i; j++) tmp = tmp->next;
		if(piles_at_pre%2)                  //neu so dong soi hien tai la le?
			take = tmp->stone -1;       // thi de lai 1 vien trong dong do
		else                                         //neu so dong hien tai la chan
			take = tmp->stone;         // thi boc het dong do
	}
   else  {   
		s = nimsum(root);
        i=1;
		if (s)        //nimsum != 0
		{
			while((tmp->stone ^ s) >= tmp->stone) 
            {
				tmp= tmp->next;
				i++;
			}
			take = tmp->stone - (tmp->stone ^ s);
		}
		else       // nimsum == 0
		{
			srand(time(NULL));
			i = pile_max(root, tmp);
			take = rand()%tmp->stone+1;
		}
	}
   tmp->stone -= take;
   if (tmp->stone==0) piles_at_pre --;
   gotoxy(30,7);
   printf(" Computer boc %d vien o dong %d.", take,i);
   getch();
}
/*=============================================*/
void normal_stragety(list &root)          //chien thuat normal cua may'
{
   list tmp = root;
   int take, s, i;       // take = so soi muon boc di
   s = nimsum(root);
   i=1;
   if (s)        //nimsum != 0
	{
		while((tmp->stone ^ s) >= tmp->stone) 
                      {
			tmp= tmp->next;
			i++;
		}
	take = tmp->stone - (tmp->stone ^ s);
	tmp->stone -= take;
	}
    else       // nimsum == 0
	{
		srand(time(NULL));
		i = pile_max(root, tmp);
		take = rand()%(tmp->stone)+1;
		tmp->stone -=take;
	}
    gotoxy(30,7);
    printf(" Computer boc %d vien o dong %d.", take,i);
    getch();
}
/*=============================================*/
int gieoxucxac()
{
   int k,i;
   clrscr();
   gotoxy(30,5);
   printf("Xin cho May gieo xuc xac: ");
   for(i=0; i<5; i++)
   {
      	printf(".");
      	sleep(1);
   }
   srand(time(NULL));
   k =rand()%6+1;
   return(k%2);
}
/*=============================================*/
void start_game_demo()
{
   clrscr();
  char ch;

   put_piles(piles_at_fisrt);
   if(test){
	 	createsolution(root, piles_at_fisrt);
     	piles_at_pre = piles_at_fisrt;
     	printf("ban co muon di truoc(c/k): ");
     	fflush(stdin);
     	scanf("%c",&ch);   
  	 	gotoxy(35,7);
     	if(ch=='c'||ch=='C') printf(" (=.=) Ban duoc di truoc!!!"); 
     	else printf(" (^_^) Computer di truoc!!!");
	}
	else
    { 
		create(root, piles_at_fisrt);
	    piles_at_pre = piles_at_fisrt;
   		status = gieoxucxac();
   		gotoxy(30,7);
   		if (status) printf(" (=.=) Ban duoc di truoc!!!");
   		else printf(" (^_^) Computer di truoc!!!");
	}
   gotoxy(30,9);
   printf(" Press any key to continue...");
   getch();
}
/*=============================================*/
int end_game(list root)
{
   list tmp;
   tmp = root;
   while(tmp != NULL && tmp->stone ==0) tmp = tmp->next;
   if (tmp == NULL) return 1;
   else return 0;
}
/*=============================================*/
void misere_game()
{
   int end;
  start_game_demo();
   do
   {
      	title(1);
      	display_table(root);
      	if (status) player(root);
      	else misere_stragety(root);
      	end = end_game(root);
      	if (!end) status = !status;
   } while (!end);
   clrscr();
   title(1); 
   display_table(root);
   gotoxy(33,7);
   if (status) printf(" Sorry! You lose!!!");  // nguoi boc cuoi cung => thua                                                                   
   else printf(" Congratulation! You win!!!"); //may boc cuoi cung thi may 
  											  // thua => nguoi thang
   getch();
}
/*============================================*/
void normal_game()
{
   int end;
  start_game_demo();
   do
   {
      	title(2);
      	display_table(root);
      	if (status) player(root);
      	else normal_stragety(root);
      	end = end_game(root);
      	if (!end) status = !status;
   } while (!end);
   clrscr();
   title(2); 
   display_table(root);
   gotoxy(33,7);
   if(status) printf(" Congratulation! You win!!!");      //neu nguoi boc cuoi thi nguoi thang
   else  printf(" Sorry! You lose!!!");            //neu nguoi boc cuoi thi nguoi thua
   getch();
}
/*================================================*/
void two_player(int mode)
{
   int end;
   clrscr();
   put_piles(piles_at_fisrt);
   create(root, piles_at_fisrt);
   piles_at_pre = piles_at_fisrt;
   status = gieoxucxac();
   gotoxy(30,7);
   if (status) printf(" B duoc di truoc");
   else printf(" A duoc di truoc");
   getch();
   do 
   {
   	title(mode); 
           display_table(root);
           gotoxy(27,5);
           if (status==0)  printf("Luot cua A :");
           else 	printf("Luot cua B :");
           player(root);
           end = end_game(root);
           if(!end) status= !status;
   } while (!end);
   clrscr();
   title(mode); 
   display_table(root);
   if (mode==1)		//che do misere
   	status = !status;
   gotoxy(33,7);
   if (status ==0)	// nguoi A thang
  	printf(" Congratulation! Player A win!!!");
   else printf(" Congratulation! Player B win!!!");
   getch();
}
/*========================================*/
void box(int x1,int y1,int x2,int y2)    // ham tao 1 hop vien quanh text
{
  int i;
  gotoxy(x1,y1);
  printf("*");
  for(i=x1+1;i<=x2-1;i++)
 	 printf("*");
  gotoxy(x1,y2);
  printf("*");
  for(i=x1+1;i<=x2-1;i++)
 	 printf("*");
  for(i=y1+1;i<=y2-1;i++)
     {
        gotoxy(x1,i);
        printf("*");
        gotoxy(x2,i);
        printf("*");
     }
    gotoxy(x2,y1);
    printf("*");
    gotoxy(x2,y2);
    printf("*");    
}
/*============================================*/
void welcome_scr()
{
  char ch;
  do
   {
    	clrscr();
    	box(29,6,53,14);
    	gotoxy(36,9);
    	printf(" WELCOME TO");	
    	gotoxy(37,11);
    	printf(" NIM GAME ");
    	gotoxy(30,17);
    	printf("SINH VIEN THUC HIEN:");   
    	gotoxy(35,18);
    	printf("LE DINH TU");            
    	gotoxy(32,19);
    	printf("NGUYEN HOANG THONG  ");
    	gotoxy(28,23);
    	printf(" Press ENTER to start game ");
    	ch=getch();
    }while(ch!=0x0D);
}
/*=============================================*/
int mainmenu()
{
   int k;
   do {
		clrscr();
		title(0);
		printf("\n\n\t");
		printf(" Main menu:");
		printf("\n\t\t");
		printf("\n1. One player");    printf("\n\t\t");
		printf("\n2. Two players");  printf("\n\t\t");
		printf("\n3. Solution");  printf("\n\t\t");
		printf("\n4. Help");  printf("\n\t\t");	
		printf("\n0. thoat");
		printf("\n\t Choose: "); scanf("%d",&k);
   } while (k<0 || k>5);
   return k;
}
/*==============================================*/
int play_mode()                      // chon che do choi
{
   int t;
   do
    {
		clrscr();
		title(0);
		printf("\n\n\t");
		printf(" Choose a play mode:");         
		printf("\n\t\t");
		printf("\n1. Misere mode "); printf("\n\t\t");     
		printf("\n2. Normal mode "); printf("\n\t\t");	
		printf("\n0. Back");	printf("\n\t\t");
		printf("\n\t Choose: "); scanf("%d",&t);
     } while (t<0 || t>2);
   return t;
}
/*==============================================*/

void help()                         // muc huong dan nguoi choi
{
	clrscr();
  fstream f;
	f.open("HUONG_DAN.txt", ios::in);	
	string data;
	string line;
	while (!f.eof())
	{
		getline(f, line);
		cout<<line<<endl;
	}
	f.close();
    getch();
}
/*===================================================*/
int main()
{
   int cmd , mode;
   welcome_scr();  
   do 
   {
      cmd = mainmenu();
      switch (cmd) {
      		case 1:
         	         test=false;
					  mode = play_mode();
                     if(mode == 1) misere_game();
                     else if (mode == 2) normal_game();
                     break;
            case 2:
      				  test=false;     	
         	          mode = play_mode();
         	          if (mode) two_player(mode);
         	          break;
          	case 3:
          		    mode=play_mode();
          		    test=true;
          		    if(mode == 1) misere_game();
                     else if (mode == 2) normal_game();
                     break;
        	case 4: help();
        			break;
            case 0: break;
      }
   } while (cmd);
}
