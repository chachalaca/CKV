#include <iostream>
#include <math.h>
#include <sstream>
#include <vector>
#include <string>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


using namespace std;


string getnum(short typ, string popis)// fce ktera ziska retezec-cislo od uzivatele, upravi ho, nebo vyzve k opetovnemu zadani
{
    string num;
    string znak;
    short numl;
    short i;
    
    cout<<popis;
    num="";
    cin>>num;
    numl=num.length();
    for(i=0;i<numl;i++)
    {
       if(!isdigit(num[i])) //je znak cislici?
       {
         znak=num[i]; 
         if(znak=="," or znak==".")//sjednotime oddelovace des. mist
         {
            if(typ==1)
            {
            znak=".";
            num[i]=znak[0];  
            }
            else //pokud je pozadovan integer(typ=0), nepovolime zadat retezce s des. carkou/teckou
            {
              cout<<"Zadavejte prosim pouze cela cisla."<<endl;
              return getnum(typ,popis);
            }        
         }
         else if(znak=="-"){}
         else
         {
          cout<<"Zadany retezec \""<<num<<"\" obsahuje nepripustny znak \""<<num[i]<<"\"."<<endl; 
          cout<<"Zadavejte prosim pouze ciselne hodnoty."<<endl;
          return getnum(typ,popis);   
         }                
       }                     
    } 
    return num;    
}

bool xport(double *x, char *fname, int n) // fce zapisujici koreny do souboru
{
     short i;
     fstream soubor;
     soubor.open(fname, ios::out);

     if (soubor.is_open())
     {   
         for(i=0; i<n; i++)
         {
         soubor<< x[i] << endl;
         }
         soubor.close();
         return true;    
     }
     else
     {
     return false;
     }     
}


bool opakovat(short status) // fce opakujici vykonavani programu
{
     string mode;
     if(status==0) // pokud predchozi vypocet probehl uspesne
     {
     cout<<endl<<"Spocitat dalsi soustavu? (ano/ne)";
     }
     else // pokud nastala chyba, pouzijeme jinou hlasku
     {
     cout<<endl<<"Zadat znovu? (ano/ne)";
     }
     cin>> mode;
     cout<<endl<<"-----------------------------------------------------"<<endl;
     if(mode=="ano")
     {
     return true;               
     }
     else 
     {
          exit(0);
     }
}


void uloz(double *x, short n) // funkce zprostredkovavajici zapis korenu do souboru
{
    string mode;
    short lmode;
    bool res;
    short i;
   
    cout<< endl<< endl <<"Chcete vysledky ulozit do souboru? ([jmeno souboru]/ne) ";
    cin>> mode;
    
    lmode=mode.length();
    char *fname = new char[lmode];

    for(i=0;i<=lmode;i++)
    {
    fname[i]=mode[i];
    }
    if(mode != "ne")
    {
    res = xport(x,fname,n);
    if(res)
    {
    cout<< "Koreny uspesne ulozeny do souboru "<<fname<<".";
    }
    else 
    {
         cout << "Ulozeni do souboru se nezdarilo. Zkusit znovu? (ano/ne)";
         cin >> mode;
         if(mode=="ano")
         {
         uloz(x,n);               
         }                 
    }
    }     
}

void fin(double *x, short n, double **m, double *mem) // funkce po uspesnem vypocitani soustavy. vypise koreny a "uklidi"
{
    short i;
    for(i=0;i<n;i++)
	{
		cout<< "\n x";
		cout<< (i+1);
		cout<< ": ";
		cout<< x[i];                
    }
    
    uloz(x,n); // nabidneme uzivateli ulozeni korenu

      
    delete [] x;
    delete [] mem;

    for (i = 0; i < n; i++)
    {
    delete [] m[i];
    }
    delete [] m;
}

bool vypocet(double **m, short n) // fce provadejici samotny vypocet obemi metodami dle volby uzivatele
{     
    short row;
    double *mem = new double[n+1]; // pracovni pamet
    short col;
    short cr;
    short cc0;
    char * pamet;
    double *cc = new double[3]; // pracovni pamet
    double *abs = new double[3];  // pamet pro aktualni rozdil mezi poslednimi iteracemi
    char *smem = new char[n+1]; // pracovni pamet
    double *x = new double[n+1]; // pamet pro vysledne hodnoty neznamych
    short diag; // pocitadlo posledniho radku s nulou na diagonale
    bool change;
    short i;
    short j;
    string mode;
    short dom;
    double soucet;
    double procento;
    
    
    ///////////////////////////////BLOK UPRAVA MATICE - ZACATEK/////////////////////////////////
    
    for(row=0;row<n;row++)
    {
        for(col=0;col<=n;col++)
       {    
                        change=false;
                        if((row+1)==col and m[row][col]==0)//když je na diagonále nula..
                		{
                         cout << endl << "Na diagonale v poloze [" << (row) << ";" << col << "] se vyskytla nula. \nZahajuji cyklus prohazovani radku..\n";
                         
                          for(i=0;i<n;i++)
                          {
                             change=false;
                             if(m[i][col]!=0 and m[row][i+1]!=0 and change==false)
                             {
                                 for(j=0;j<=n;j++) // prohozeni radku
                                 {
                                    mem[j]=m[row][j];
                                                                    
                                    m[row][j]=m[i][j];
                                    
                                    m[i][j]=mem[j];             
                                 }
                                 cc0=i;
                                 i=(n+1);
                                 change=true;            
                             }             
                          }
                          if(change=true)
                          {
                              cout << "Radek " << row+1 << " byl prohozen s radkem " << cc0+1 << endl;      
                          }
                          change=false;                                    
                        }            
                }
    }
    
    ///////////////////////////////BLOK UPRAVA MATICE - KONEC/////////////////////////////////
    
    cout << endl << "Chcete pro vypocet pouzit gaussovu, nebo iteracni metodu? (gauss/iter) ";
    cin >> mode;
    
    ///////////////////////////////BLOK GAUSSOVA ELIMINACE - ZACATEK/////////////////////////////////
          
     if(mode == "gauss")
     {          
                diag = n+1; // pocitadlo radku s nulou na diagonale nastavime na vychozi hodnotu
                for(row=1;row<n;row++) // jedeme po radkach
                {
                	for(col=1;col<=row;col++) // ..a po sloupcich
                	{
                		
               		if(m[row][col] != 0)
               		{
                        cc0 = col+1;
                        for(cr=0;cr<row;cr++) // hledame vhodnou radku na odecteni..
                		{
                			if(m[cr][col]!=0)
                			{
                                cc0=cr; // cislo vhodneho radku na odecteni
                				cc[1]=m[cr][col]; // hodnota odpovidajici bunky na vhodnem radku
                			}
                		}
                		if(cc0 == col+1)
                        {
                        cout<< "CHYBA! Pro radku "<<row<<" nebyla nalezena vhodna radka na odecteni.";
                        return false;
                        }
                        //if(cc[3]!=0)
                        //{
                		cc[3]=m[row][col];// hodnota aktualni bunky
                        //}
                		for(i=0; i <= n; i++) // rozsireni aktualni radky
                		{
                			m[row][i] = cc[1]*m[row][i] ;
                		}
    
                		for(i=0; i <= n; i++) // rozsireni odecitane radky
                		{
                			//if(cc[3]!=0)
                            //{
                            m[cc0][i] = m[cc0][i]*cc[3] ;
                            //}
                		}
                        cout << "Odecitam od sebe radky " << row << " a " << cc0 << endl;
                		for(i=0; i <= n; i++) // odecteni radek od sebe
                		{
                			m[row][i] = (m[row][i] - m[cc0][i]) ;
                		}
                		for(i=0; i <= n; i++) // zpetne vykraceni odecitane radky
                		{
                			//if(cc[3]!=0)
                            //{
                            m[cc0][i] = m[cc0][i]/cc[3] ;
                            //}
                		}
                    }
                    }
                if(m[row][row+1]==0)
                {
                diag = row;
                }
                }
                if(diag != n+1)
                {
                cout<< "CHYBA! Pro nezname x1 - x"<<row+1<<" nebylo nalezeno neparametricke reseni.";
                return false;
                }
    			for(i=n-1;i>=0;i--) // hledame hodnoty neznamych postupnym dosazovanim
    			{
    				for(j=i+2;j<=n;j++)
    				{
    				m[i][0]=(m[i][0]-(x[(j-1)]*m[i][j]));
    				}
    				x[i]=m[i][0]/m[i][i+1];
    				}
    
                cout << endl << "-----------------------------------------------" << "\nVYSLEDKY Z GAUSSOVY ELIMINACE";
    			fin(x,n,m,mem); 
                return true;   
    }
    ///////////////////////////////BLOK GAUSSOVA ELIMINACE - KONEC///////////////////////////////// 
    
    ///////////////////////////////BLOK ITERACNI METODA - ZACATEK/////////////////////////////////
    if(mode == "iter")
    {
    
    //ANALYZA ZDA JE MATICE DIAGONALNE DOMINANTNI
     dom = 0; //pocitadlo diagonalne dominantnich radku
     for(row=0;row<n;row++)//projidime matici po radcich
     {
    	soucet=0;
        for(col=1;col<=n;col++) // ..po sloupcich
    	 {
         soucet += m[row][col];
         }
        if((soucet - m[row][row+1]) < m[row][row+1])
          {
          dom += 1; //pokud je radka v ramci matice diagonalne dominantni, zvysime pocet d.d. radek o 1         
          }
     }
     procento = ((dom/n)*100);
     cout<< "\n\nPocet diagonalne dominantnich radku: " << dom << "/" << n << " (" << procento << "%)";
     
     if(dom < n)
     {
     cout<< "\nJe nam lito, ale matice neni diagonalne dominantni. \nZkuste soustavu vypocitat gaussovou eliminaci.\n Pokracovat? (ano/ne)";       
     cin >> mode;
     if(mode == "ano")
             {
             vypocet(m,n);
             }
     else exit(0);
     }
     
     //pokud matice je diagonalne dominantni, upravime ji na pozadovany tvar
     if(dom >= n and mode == "iter")
     {
            for(row=0;row<n;row++) // projizdime matici po radcich
            {
              cc[2] = m[row][row+1]; // do pameti si ulozime cislo na diagonale
              for(col=0;col<=n;col++) // projizdime postupne prvky v radce
              {
              m[row][col] = (m[row][col] / cc[2]); // radek vydelime cislem na diagonale, tim dostaneme na diagonalu jednicku
              if(col >= 1)
                {
                m[row][col] = -(m[row][col]); // pomyslne prevedeni na druhou stranu rovnice
                }
              }
              m[row][row+1] = 0;
            }     
     
     //naplnime nezname pocatecnimi hodnotami
     for(i=0;i<n;i++)
     {
       x[i] = 1;               
     }
     
     //zahajime iteracni proces
     abs[0] = 1;
     abs[1] = 1;
     for(i=0; (abs[0] >= 0.00001 or abs[1] >= 0.00001) and i<=50 ;i++)  //opakovat iterace dokud hodnoty nebudou dostatecne konvergovat, nebo nebude dosazeno poctu 50 opakovani
     {
           cout << "\n\nITERACE CISLO " << i+1;
           for(row=0;row<n;row++)
            {
              mem[row] = x[row]; // pro porovnani si ulozime hodnotu z predchozi iterace
              x[row]=m[row][0]; // pripocteme absolutni clen daneho radku
              for(col=1;col<=n;col++)
              {   
              x[row] += (m[row][col] * x[col-1]); //zde pripocteme cleny s neznamymi do nichz dosadime
              }
              cout << "\nX" << row+1 << ": " << x[row];
            }
          abs[0] = fabs(mem[0]/x[0]-1); // spocteni rozdilu hodnot vzorkovych neznamych mezi jednotlivymi iteracemi
          abs[1] = fabs(mem[n-1]/x[n-1]-1);  
     } 
        cout<< "\n\n------------------------------------------\nVYSLEDKY Z ITERACNI METODY";
        fin(x,n,m,mem);
        return true;
     } 
     }
    ///////////////////////////////BLOK ITERACNI METODA - KONEC///////////////////////////////// 
} //fce vypocet - konec

bool nacti(short status)
{
    short i; // pocitadlo
    short j; // pocitadlo
    short n; // rozmer matice-pocet rovnic
    string mode; // zprostredkovava volbu uzivatele
    string upload; // zprostredkovava volbu uzivatele- rucni rezim/ze souboru
    string radek;
    double **m; // matice
    short lmode;
    
    ///////////////////////////////BLOK UPLOAD - ZACATEK/////////////////////////////////  
    if(status==1)
    {
    cout<< endl<< endl <<"Zadat jmeno souboru znovu? ([jmeno souboru]/ne) ";
    }
    else
    {
    cout << "Pokud chcete nacist soustavu ze souboru, pripravte si do stejneho adresare \nzdrojovy soubor techto parametru: \n napr. pro soustavu \n\n a1*x1 + b1*x2 = c1 \n a2*x1 + b2*x2 = c2 \n\n by byl format souboru nasledujici: \n\n c1 a1 b1 \n c2 a2 b2 \n\nJednotlive hodnoty jsou oddeleny mezerami, \nVSECHNY (i posledni) konce radku odlomenim.\n\n";
    cout<< endl<< endl <<"Chcete tedy nacist soustavu ze souboru? ([jmeno souboru]/ne) ";
    }
    cin>> mode;
        
    lmode=mode.length();
    char *fname = new char[lmode];

    for(i=0;i<=lmode;i++) // ze stringu potrebujeme char, abychom jej mohli pouzit jako jmeno souboru
    {
    fname[i]=mode[i];
    }
    if(mode != "ne")
    {               
    ifstream soubor (fname); // otevreme soubor
    if (soubor.is_open()) 
    {
       double **m;
       short i=0, j,n;
       stringstream ss;
       double cislo;
       vector<double> hodnoty; //hodnoty v radku 

       while((!getline(soubor, radek).eof()) && (radek != ""))
       {
        
        i++;
        cout << i;
        cout << ". radek: ";
        cout << radek;
        ss.str(radek);

        while(!ss.eof())
        {
         ss>> cislo;
         hodnoty.push_back(cislo);             
        }
        ss.clear();

        if(i==1)
        {                     
              n=hodnoty.size(); // ziskame rozmer matice
              cout << ", n: ";
              cout << n-1;

              m = new double*[n];
              for (j = 0; j < n; j++)
              {
              m[j] = new double[n];
              }
        }        
        cout << ", hodnoty:";
        for(j=0; j<n; j++)
        {
         m[i-1][j]=  hodnoty[j];
         cout << " ";
         cout << m[i-1][j];   
        }
        hodnoty.clear();
        cout << endl;
      } 
        soubor.close();
        cout << endl;
        n--;   

        // pokud se povedlo, zahajime vypocet
        if(!vypocet(m,n)){if(opakovat(1)){return false;}else return true;} else{if(opakovat(0)){return false;}else return true;}                 
               
    }
    else {
         cout << "Nepodarilo se otevrit soubor. "<<endl<<"Zkontrolujte zda zadavate spravne jeho nazev."<<endl; 
         nacti(1);
         }
              }
    ///////////////////////////////BLOK UPLOAD - KONEC/////////////////////////////////
}

void base() // fce zprostredkovavajici sber dat
{ 
    short i; // pocitadlo
    short j; // pocitadlo
    short row; // pocitadlo radek - X souradnice na matici, rozsah <0;n)
    short n; // rozmer matice-pocet rovnic
    short col; // pocitadlo sloupcu matice - Y souradnice na matici, rozsah <0;n>, bunky [*][0] obsahuji hodnotu absolutniho clenu, bunky [*][a] kde a je z <1;n> obsahuji koeficienty neznamych 
    string mode; // zprostredkovava volbu uzivatele
    string upload; // zprostredkovava volbu uzivatele- rucni rezim/ze souboru
    double **m; // matice
    short lmode;


    if(!nacti(0)){base();}

    ///////////////////////////////BLOK SBERU DAT - ZACATEK/////////////////////////////////
    if (upload != "ano")
    {    
         n = atoi(getnum(0, "Zadejte prosim rucne pocet rovnic a neznamych: ").c_str());
         
         string popis;         
         char *buf;
         double **m;
         m = new double*[n+1];
         for (i = 0; i < n+1; i++)
             {
             m[i] = new double[n+1];
             }
    
             // RUCNI ZADAVANI ROVNIC
             for (i=0; i<n; i++)
    	     {
    	     for (j=0; j<=n; j++)
    		 {
    		 popis = "";
             std::string buf; // konverze short --> string
             std::stringstream out;
             out << i;
             buf = out.str();
             popis += buf;
    		 popis += "x";
    		 if (j==0)
    		 {
    		 popis += "a";
    		 }
    		 else 
             {
             std::string buf;// konverze short --> string
             std::stringstream out;
             out << j;
             buf = out.str();
             popis += buf;
             }
    		 popis += ":";

    		 m[i][j]=strtod(getnum(1,popis).c_str(),NULL); // ziskame string-double, konvertujeme jej na double, a zapiseme do matice
    		 }
    	     }      
    
    ///////////////////////////////BLOK SBERU DAT - KONEC/////////////////////////////////  
    
    // pokud se povedlo, zahajime vypocet
    if(!vypocet(m,n)){if(opakovat(1)){nacti(0);}} else{if(opakovat(0)){nacti(0);}}
        
    }
}    

int main()
{
    cout << "PROGRAM PRO RESENI n LINEARNICH ROVNIC O n NEZNAMYCH\n";
    cout << "(c) 2010 FRANTISEK BLACHOWICZ\n\n\n";

    base();

    cin.get();
    cin.get();
}



