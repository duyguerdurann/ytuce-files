// verilen n*m boyutundaki bir matrisin icinde p*q boyutundaki alt matrisleri bulan program
#include <stdio.h>
int main()
{
	char mat[100][100], amat[20][20];
	int n,m,p,q,i,j,k,l,top,altMatrisSayi=0;
	printf("Ilk matrisin eleman sayisini veriniz\n");
	scanf("%d %d",&n,&m);
	printf("Ilk matrisi veriniz\n");
	for (i=0;i<n;i++)
		for (j=0;j<m;j++)
			scanf("%d",&mat[i][j]);
	printf("Ikinci matrisin eleman sayisini veriniz\n");
	scanf("%d %d",&p,&q);
	printf("Ikinci matrisi veriniz\n");
	for (i=0;i<p;i++)
		for (j=0;j<q;j++)
			scanf("%d",&amat[i][j]);
	for (i=0;i<n-p+1;i++)
		for (j=0;j<m-q+1;j++)
		{	
			top=0;
			for (k=0;k<p;k++)	
				for (l=0;l<q;l++)
				{
					top=top+(mat[i+k][j+l]^amat[k][l]);
				}
			if (top==0)
			{
				printf("%d %d\n",i,j);
				altMatrisSayi++;
			}
		}
		
	printf("alt matris:%d\n",altMatrisSayi);			
}
