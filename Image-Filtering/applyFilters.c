#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.14

typedef struct pgm{
	char format[2];
	int row;
	int col;
	int max_gray;
	int arr[600][600];
}PGM;


// READING - WRITING PGM FILES
void readPgm_P2(PGM *pgmformat, char dosya[]){
	char sakla[50];
	
	FILE* fp = fopen(dosya, "r");
	
	if(fp == NULL){
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}
	
	fscanf(fp,"%s", &pgmformat->format);
	
	fscanf(fp, "%s", sakla);
	if(sakla[0] == '#'){
		fgets(sakla, 50, fp);
		fscanf(fp, "%s", sakla);
	}
	
    sscanf(sakla, "%d", &pgmformat->col);
    
    fscanf(fp, "%d", &pgmformat->row);
    
    fscanf(fp,"%d", &pgmformat->max_gray);
    
    int i,j;
    for(i=0;i<pgmformat->row;i++){
    	for(j=0;j<pgmformat->col;j++){
            if(fscanf(fp, "%d", &pgmformat->arr[i][j]) != 1){
            	exit(1);	
			}
		}
	}	
}


void turnPgm_P2(PGM *pgmformat, char dosya[]){
	int row, column, i, j;
	
	FILE* fp = fopen(dosya, "w");
	
	fprintf(fp, "%s", pgmformat->format);
	fprintf(fp, "\n");
	
	column = pgmformat->col;
	row = pgmformat->row;
	
	fprintf(fp, "%d %d", column, row);
	fprintf(fp, "\n");
	
	fprintf(fp, "%d", pgmformat->max_gray);
	fprintf(fp, "\n");

	for(i=0;i<row;i++){
		for(j=0;j<column;j++){
			fprintf(fp, "%d ", pgmformat->arr[i][j]);
		}
	}
	
	//printf("\n>>> Dosyaya yazma islemi gerceklestirildi.\n");
	
	fclose(fp);
}


void readPgm_P5(PGM *pgmformat, char dosya[]){
	char tut[30], tut2[100], sakla[100];
	int a;
	
	FILE* fp = fopen(dosya, "rb");
	
    if(fp == NULL){
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}
	
	fscanf(fp,"%s", &pgmformat->format);
	//printf("TUT: %s", pgmformat->format);
	
	fscanf(fp, "%s", &sakla);
	//printf("\nSaklaaa: %s", sakla);
	
	if(sakla[0] == '#'){
		fgets(sakla, 100, fp);
		fscanf(fp, "%s", sakla);
	}
	
    sscanf(sakla, "%d", &pgmformat->col);
    //printf("\nCOL: %d", pgmformat->col);
    
    fscanf(fp, "%d", &pgmformat->row);
    //printf("\nROW: %d", pgmformat->row);
    
    fscanf(fp,"%d", &pgmformat->max_gray);
    //printf("\naaa: %d", pgmformat->max_gray);
    //printf("\n\n\nMerhabaa\n\n\n");
    
    int i,j, lo;
    for(i=0;i<pgmformat->row;i++){
    	for(j=0;j<pgmformat->col;j++){
    		lo = fgetc(fp);
            pgmformat->arr[i][j] = lo;
		}
	}
	fclose(fp);
}


void turnPgm_P5(PGM *pgmformat, char dosya[]){
	int row, column, i, j;
	int a,b;
	
	FILE* fp = fopen(dosya, "wb");
	
	fprintf(fp, "%s", pgmformat->format);
	fprintf(fp, "\n");
	
	row = pgmformat->row;
	column = pgmformat->col;
	
	fprintf(fp, "%d %d", column, row);
	fprintf(fp, "\n");
	
	fprintf(fp, "%d", pgmformat->max_gray);
	fprintf(fp, "\n");
	
	
	int lo;
	for(i=0;i<row;i++){
		for(j=0;j<column;j++){
			lo = pgmformat->arr[i][j];
            fputc(lo, fp);
		}
	}
	
	//printf("\n>>> Dosyaya yazma islemi gerceklestirildi.\n");
	
	fclose(fp);
}


PGM *normalization(PGM *pgmformat){
	int i,j, max=-1, min=10000;
	
	for (i=0;i<pgmformat->row;i++){
		for(j=0;j<pgmformat->col;j++){
			if (pgmformat->arr[i][j]>max){
	        	max = pgmformat->arr[i][j];	
	        }
		}
	}
	
	//printf("Max: %d", max);
	
    for(i=0;i<pgmformat->row;i++){
    	for(j=0;j<pgmformat->col;j++){
	    	if(pgmformat->arr[i][j]<min){
	    		min = pgmformat->arr[i][j];	
			}	
		}
    }
    
    //printf("Min: %d", min);
	float tut;
	
	for(i=0;i<pgmformat->row;i++){
		for(j=0;j<pgmformat->col;j++){
			tut = (pgmformat->arr[i][j]-min) / (float)(max-min);
			pgmformat->arr[i][j] = (int)(tut*255);
		}
	}
	
	return pgmformat;
}


// GAUSS FILTERS

void gauss_filters(int kernel_size, float sigma, PGM *pgmformat, int flag, char filename[]){
	double gauss[kernel_size][kernel_size];
	double v;
	int x,y,i,j,a,sakla1,sakla2,m,n;
	double sum;
	PGM *pgmformat_2 = (PGM*)malloc(sizeof(PGM));
	memcpy(pgmformat_2, pgmformat, sizeof(PGM));
	
	
	v = 1 / (2*PI*pow(sigma,2));
	
	for(i=0;i<kernel_size;i++){
        for(j=0;j<kernel_size;j++){
            x = i - (kernel_size-1) / 2.0;
            y = j - (kernel_size-1) / 2.0;
            gauss[i][j] = v * exp(((pow(x,2) + pow(y,2)) / ((2 * pow(sigma,2)))) * (-1));
            sum += gauss[i][j];
        }
    }

	printf("\n%dx%d boyutunda ve sigmasi %d olan kernel\n", kernel_size, kernel_size, (int)sigma);
	for(i=0;i<kernel_size;i++){
		for(j=0;j<kernel_size;j++){
			gauss[i][j] /= sum;
			printf("%f ", gauss[i][j]);		
		}
		printf("\n");
	}
	
	a = (kernel_size-1);
	
	// Kerneli resme uygulama
	for(i=0;i<pgmformat->row - a;i++){
		for(j=0;j<pgmformat->col - a;j++){
			sakla1 = i + (kernel_size-1) / 2;
            sakla2 = j + (kernel_size-1) / 2;
            sum = 0;
            for(m=0;m<kernel_size;m++){
				for(n=0;n<kernel_size;n++){
					sum += (pgmformat->arr[i+m][j+n] * gauss[m][n]);
				}
			}
			pgmformat_2->arr[sakla1][sakla2] = (int)sum; // renk degeri float, double olamaz. int olmali.
		}
	}
	/*
	for(i=0;i<pgmformat_2->row;i++){
		for(j=0;j<pgmformat_2->col;j++){
			printf("%d ", pgmformat_2->arr[i][j]);
		}
		printf("\n");
	}
	*/
	
	if (flag == 1){
		turnPgm_P5(pgmformat_2, filename);
	}
	else if (flag == 2){
		turnPgm_P2(pgmformat_2, filename);
	}
}


void sobel_edge_detector(PGM *pgmformat, int flag, char filename[]){
	double sum_x, sum_y;
	int i,j,x,y, sakla1,sakla2,m,n;
	char filename1[30], filename2[30], filename3[30];
	
	double mx[3][3] = {
		{-1,0,1},
		{-2,0,2},
		{-1,0,1}
	};
	double my[3][3] = {
		{-1,-2,-1},
		{0,0,0},
		{1,2,1}
	};
	
	PGM *pgmformat_2 = (PGM*)malloc(sizeof(PGM));
	memcpy(pgmformat_2, pgmformat, sizeof(PGM));
	
	PGM *pgmformat_3 = (PGM*)malloc(sizeof(PGM));
	memcpy(pgmformat_3, pgmformat, sizeof(PGM));
	
	PGM *pgmformat_4 = (PGM*)malloc(sizeof(PGM));
	memcpy(pgmformat_4, pgmformat, sizeof(PGM));
	
	for(i=0;i<pgmformat->row - 2;i++){
		for(j=0;j<pgmformat->col - 2;j++){

			sakla1 = i + 1;
            sakla2 = j + 1;
			sum_x=0;
			for(m=0;m<3;m++){
				for(n=0;n<3;n++){
					sum_x += (pgmformat->arr[m+i][n+j] * mx[m][n]);
				}
			}
			sum_y=0;
			for(m=0;m<3;m++){
				for(n=0;n<3;n++){
					sum_y += (pgmformat->arr[m+i][n+j] * my[m][n]);
				}
			}
			
			pgmformat_2->arr[sakla1][sakla2] = (int)sqrt(sum_x*sum_x + sum_y*sum_y);// renk degeri float, double olamaz. int olmali.
			//printf("%d ", pgmformat_2->arr[sakla1][sakla2]);
			pgmformat_3->arr[sakla1][sakla2] = (int)sum_x;
			pgmformat_4->arr[sakla1][sakla2] = (int)sum_y;
		}
	}
	char temp[30];
	strcpy(temp, filename);
	strcpy(filename1, strcat(temp, "_xy.pgm"));
	strcpy(temp, filename);
	strcpy(filename2, strcat(temp, "_x.pgm"));
	strcpy(temp, filename);
	strcpy(filename3, strcat(temp, "_y.pgm"));
	
	pgmformat_2 = normalization(pgmformat_2);
	pgmformat_3 = normalization(pgmformat_3);
	pgmformat_4 = normalization(pgmformat_4);

	
	if (flag == 1){
		//printf("\nSobel fitresi x ve y yonunde degisim uygulandi.");
		turnPgm_P5(pgmformat_2, filename1);
		//printf("\nSobel fitresi x yonunde degisim uygulandi.");
		turnPgm_P5(pgmformat_3, filename2);
		//printf("\nSobel fitresi y yonunde degisim uygulandi.");
		turnPgm_P5(pgmformat_4, filename3);
	}
	else if (flag == 2){
		//printf("\nSobel fitresi x ve y yonunde degisim uygulandi.");
		turnPgm_P2(pgmformat_2, filename1);
		//printf("\nSobel fitresi x yonunde degisim uygulandi.");
		turnPgm_P2(pgmformat_3, filename2);
		//printf("\nSobel fitresi y yonunde degisim uygulandi.");
		turnPgm_P2(pgmformat_4, filename3);
	}
}

void laplacian_filter(PGM *pgmformat, int flag, char filename[]){
	int sum_1, sum_2;
	int i,j,x,y, sakla1,sakla2,m,n;
	char filename1[30], filename2[30];
	
	int l_kernel_1[3][3] = {
		{0,-1,0},
		{-1,4,-1},
		{0,-1,0}
	};
	int l_kernel_2[3][3] = {
		{-1,-1,-1},
		{-1,8,-1},
		{-1,-1,-1}
	};
	
	PGM *pgmformat_2 = (PGM*)malloc(sizeof(PGM));
	memcpy(pgmformat_2, pgmformat, sizeof(PGM));
	
	PGM *pgmformat_3 = (PGM*)malloc(sizeof(PGM));
	memcpy(pgmformat_3, pgmformat, sizeof(PGM));

	
	for(i=0;i<pgmformat->row - 2;i++){
		for(j=0;j<pgmformat->col - 2;j++){

			sakla1 = i + 1;
            sakla2 = j + 1;
			sum_1=0;
			for(m=0;m<3;m++){
				for(n=0;n<3;n++){
					sum_1 += (pgmformat->arr[m+i][n+j] * l_kernel_1[m][n]);
				}
			}
			sum_2=0;
			for(m=0;m<3;m++){
				for(n=0;n<3;n++){
					sum_2 += (pgmformat->arr[m+i][n+j] * l_kernel_2[m][n]);
				}
			}
			
			pgmformat_2->arr[sakla1][sakla2] = (int)sum_1;// renk degeri float, double olamaz. int olmali.
			//printf("%d ", pgmformat_2->arr[sakla1][sakla2]);
			pgmformat_3->arr[sakla1][sakla2] = (int)sum_2;
		}
	}
	
	pgmformat_2 = normalization(pgmformat_2);
	pgmformat_3 = normalization(pgmformat_3);
	
	char temp[30];
	strcpy(temp, filename);
	strcpy(filename1, strcat(temp, "_k1.pgm"));
	strcpy(temp, filename);
	strcpy(filename2, strcat(temp, "_k2.pgm"));
	
	if (flag == 1){
		//printf("\nLaplacian kernel 1 uygulandi.");
		turnPgm_P5(pgmformat_2, filename1);
		//printf("\nLaplacian kernel 2 uygulandi.");
		turnPgm_P5(pgmformat_3, filename2);
	}
	else if (flag == 2){
		//printf("\nLaplacian kernel 1 uygulandi.");
		turnPgm_P2(pgmformat_2, filename1);
		//printf("\nLaplacian kernel 2 uygulandi.");
		turnPgm_P2(pgmformat_3, filename2);
	}
}


int main(){
	char dosya[30], dosya2[30], kontrol[30];
	int i,j, flag, numara;
	char filename[30];

	FILE *fp;
	PGM *pgmformat = (PGM*)malloc(sizeof(PGM));
	PGM *filteredpgm = (PGM*)malloc(sizeof(PGM));
	
	
	printf("PROGRAMA HOSGELDINIZ !\n\n");
	printf("Lutfen pgm dosyasinin adini giriniz (file.pgm) : ");
	scanf("%s", dosya);
	
	if(fopen(dosya, "rb") == NULL){
		printf("Error while opening the file.\n");
		exit(1);
	}
	else{
		fp = fopen(dosya, "rb");
		fscanf(fp,"%s", &kontrol);
		if(strcmp(kontrol, "P5") != 0 & strcmp(kontrol, "P2") != 0){
			printf("Girilen bir pgm format degildir.\n");
		}
		else if(strcmp(kontrol, "P5") == 0){
			printf("\n>>> PGM P5 dosyasi okundu.\n");
			readPgm_P5(pgmformat, dosya);
			flag=1;

		}
		else if(strcmp(kontrol, "P2") == 0){
			printf("\n>>> PGM P2 dosyasi okundu.\n");
			readPgm_P2(pgmformat, dosya);
			flag=2;
		}
	}
	
	int k[3] = {3,5,7};
	float s[3] = {1.0,2.0,4.0};
	
	printf("\n1- GAUSS FILTRESI UYGULAMA\n");
	printf("\n----------------------------------------------------------------------------\n");
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			sprintf(filename, "gauss_%d_%d.pgm", k[i],(int)s[j]);
			gauss_filters(k[i], s[j],pgmformat, flag, filename);
		}
		printf("\n----------------------------------------------------------------------------\n");
	}
	
	printf("\n2- SOBEL FILTRESI UYGULAMA\n");
	strcpy(filename, "sobel_original");
	sobel_edge_detector(pgmformat, flag, filename);
	printf("\nOrijinal goruntulere sobel filtresi uygulama islemi tamamlandi.\n");
	
	printf("\n----------------------------------------------------------------------------\n");
	
	printf("\n3- BULANIKLASTIRILMIS GORUNTULERE SOBEL FILTRESI UYGULAMA\n");
	
	
	if (flag==1){
		strcpy(filename, "sobel_blurred_3_1");
		strcpy(dosya2, "gauss_3_1.pgm");
		readPgm_P5(filteredpgm, dosya2);
		sobel_edge_detector(filteredpgm, flag, filename);
		
		strcpy(filename, "sobel_blurred_3_2");
		strcpy(dosya2, "gauss_3_2.pgm");
		readPgm_P5(filteredpgm, dosya2);
		sobel_edge_detector(filteredpgm, flag, filename);

		strcpy(filename, "sobel_blurred_3_4");
		strcpy(dosya2, "gauss_3_4.pgm");
		readPgm_P5(filteredpgm, dosya2);
		sobel_edge_detector(filteredpgm, flag, filename);
		
		strcpy(filename, "sobel_blurred_5_1");
		strcpy(dosya2, "gauss_5_1.pgm");
		readPgm_P5(filteredpgm, dosya2);
		sobel_edge_detector(filteredpgm, flag, filename);
		
		strcpy(filename, "sobel_blurred_5_2");
		strcpy(dosya2, "gauss_5_2.pgm");
		readPgm_P5(filteredpgm, dosya2);
		sobel_edge_detector(filteredpgm, flag, filename);
		
		strcpy(filename, "sobel_blurred_5_4");
		strcpy(dosya2, "gauss_5_4.pgm");
		readPgm_P5(filteredpgm, dosya2);
		sobel_edge_detector(filteredpgm, flag, filename);
		
		strcpy(filename, "sobel_blurred_7_1");
		strcpy(dosya2, "gauss_7_1.pgm");
		readPgm_P5(filteredpgm, dosya2);
		sobel_edge_detector(filteredpgm, flag, filename);
		
		strcpy(filename, "sobel_blurred_7_2");
		strcpy(dosya2, "gauss_7_2.pgm");
		readPgm_P5(filteredpgm, dosya2);
		sobel_edge_detector(filteredpgm, flag, filename);
		
		strcpy(filename, "sobel_blurred_7_4");
		strcpy(dosya2, "gauss_7_4.pgm");
		readPgm_P5(filteredpgm, dosya2);
		sobel_edge_detector(filteredpgm, flag, filename);
	}
	else if(flag==2){
		strcpy(filename, "sobel_blurred_3_1");
		strcpy(dosya2, "gauss_3_1.pgm");
		readPgm_P2(filteredpgm, dosya2);
		sobel_edge_detector(filteredpgm, flag, filename);
		
		strcpy(filename, "sobel_blurred_3_2");
		strcpy(dosya2, "gauss_3_2.pgm");
		readPgm_P2(filteredpgm, dosya2);
		sobel_edge_detector(filteredpgm, flag, filename);

		strcpy(filename, "sobel_blurred_3_4");
		strcpy(dosya2, "gauss_3_4.pgm");
		readPgm_P2(filteredpgm, dosya2);
		sobel_edge_detector(filteredpgm, flag, filename);
		
		strcpy(filename, "sobel_blurred_5_1");
		strcpy(dosya2, "gauss_5_1.pgm");
		readPgm_P2(filteredpgm, dosya2);
		sobel_edge_detector(filteredpgm, flag, filename);
		
		strcpy(filename, "sobel_blurred_5_2");
		strcpy(dosya2, "gauss_5_2.pgm");
		readPgm_P2(filteredpgm, dosya2);
		sobel_edge_detector(filteredpgm, flag, filename);
		
		strcpy(filename, "sobel_blurred_5_4");
		strcpy(dosya2, "gauss_5_4.pgm");
		readPgm_P2(filteredpgm, dosya2);
		sobel_edge_detector(filteredpgm, flag, filename);
		
		strcpy(filename, "sobel_blurred_7_1");
		strcpy(dosya2, "gauss_7_1.pgm");
		readPgm_P2(filteredpgm, dosya2);
		sobel_edge_detector(filteredpgm, flag, filename);
		
		strcpy(filename, "sobel_blurred_7_2");
		strcpy(dosya2, "gauss_7_2.pgm");
		readPgm_P2(filteredpgm, dosya2);
		sobel_edge_detector(filteredpgm, flag, filename);
		
		strcpy(filename, "sobel_blurred_7_4");
		strcpy(dosya2, "gauss_7_4.pgm");
		readPgm_P2(filteredpgm, dosya2);
		sobel_edge_detector(filteredpgm, flag, filename);
	}
	
	printf("\nBulaniklastirilmis goruntulere sobel filtresi uygulama islemi tamamlandi.\n");
	printf("\n----------------------------------------------------------------------------\n");
	printf("\n4- BULANIKLASTIRILMIS GORUNTULERE LAPLACIAN FILTRESI UYGULAMA\n");
	
	if (flag==1){
		strcpy(filename, "lap_blurred_3_1");
		strcpy(dosya2, "gauss_3_1.pgm");
		readPgm_P5(filteredpgm, dosya2);
		laplacian_filter(filteredpgm, flag, filename);
		
		strcpy(filename, "lap_blurred_3_2");
		strcpy(dosya2, "gauss_3_2.pgm");
		readPgm_P5(filteredpgm, dosya2);
		laplacian_filter(filteredpgm, flag, filename);

		strcpy(filename, "lap_blurred_3_4");
		strcpy(dosya2, "gauss_3_4.pgm");
		readPgm_P5(filteredpgm, dosya2);
		laplacian_filter(filteredpgm, flag, filename);
		
		strcpy(filename, "lap_blurred_5_1");
		strcpy(dosya2, "gauss_5_1.pgm");
		readPgm_P5(filteredpgm, dosya2);
		laplacian_filter(filteredpgm, flag, filename);
		
		strcpy(filename, "lap_blurred_5_2");
		strcpy(dosya2, "gauss_5_2.pgm");
		readPgm_P5(filteredpgm, dosya2);
		laplacian_filter(filteredpgm, flag, filename);
		
		strcpy(filename, "lap_blurred_5_4");
		strcpy(dosya2, "gauss_5_4.pgm");
		readPgm_P5(filteredpgm, dosya2);
		laplacian_filter(filteredpgm, flag, filename);
		
		strcpy(filename, "lap_blurred_7_1");
		strcpy(dosya2, "gauss_7_1.pgm");
		readPgm_P5(filteredpgm, dosya2);
		laplacian_filter(filteredpgm, flag, filename);
		
		strcpy(filename, "lap_blurred_7_2");
		strcpy(dosya2, "gauss_7_2.pgm");
		readPgm_P5(filteredpgm, dosya2);
		laplacian_filter(filteredpgm, flag, filename);
		
		strcpy(filename, "lap_blurred_7_4");
		strcpy(dosya2, "gauss_7_4.pgm");
		readPgm_P5(filteredpgm, dosya2);
		laplacian_filter(filteredpgm, flag, filename);
	}
	else if(flag==2){
		strcpy(filename, "lap_blurred_3_1");
		strcpy(dosya2, "gauss_3_1.pgm");
		readPgm_P2(filteredpgm, dosya2);
		laplacian_filter(filteredpgm, flag, filename);
		
		strcpy(filename, "lap_blurred_3_2");
		strcpy(dosya2, "gauss_3_2.pgm");
		readPgm_P2(filteredpgm, dosya2);
		laplacian_filter(filteredpgm, flag, filename);

		strcpy(filename, "lap_blurred_3_4");
		strcpy(dosya2, "gauss_3_4.pgm");
		readPgm_P2(filteredpgm, dosya2);
		laplacian_filter(filteredpgm, flag, filename);
		
		strcpy(filename, "lap_blurred_5_1");
		strcpy(dosya2, "gauss_5_1.pgm");
		readPgm_P2(filteredpgm, dosya2);
		laplacian_filter(filteredpgm, flag, filename);
		
		strcpy(filename, "lap_blurred_5_2");
		strcpy(dosya2, "gauss_5_2.pgm");
		readPgm_P2(filteredpgm, dosya2);
		laplacian_filter(filteredpgm, flag, filename);
		
		strcpy(filename, "lap_blurred_5_4");
		strcpy(dosya2, "gauss_5_4.pgm");
		readPgm_P2(filteredpgm, dosya2);
		laplacian_filter(filteredpgm, flag, filename);
		
		strcpy(filename, "lap_blurred_7_1");
		strcpy(dosya2, "gauss_7_1.pgm");
		readPgm_P2(filteredpgm, dosya2);
		laplacian_filter(filteredpgm, flag, filename);
		
		strcpy(filename, "lap_blurred_7_2");
		strcpy(dosya2, "gauss_7_2.pgm");
		readPgm_P2(filteredpgm, dosya2);
		laplacian_filter(filteredpgm, flag, filename);
		
		strcpy(filename, "lap_blurred_7_4");
		strcpy(dosya2, "gauss_7_4.pgm");
		readPgm_P2(filteredpgm, dosya2);
		laplacian_filter(filteredpgm, flag, filename);
	}
	printf("\nBulaniklastirilmis goruntulere laplacian filtresi uygulama islemi tamamlandi.\n");
	
	printf("\n\n>>> PROGRAM SONLANMISTIR.\n");
	return 0;
}


