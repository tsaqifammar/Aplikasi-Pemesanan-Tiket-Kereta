#ifndef HEADER_H
#define HEADER_H

#include<iostream>
#include<stdlib.h>
#include<conio.h>
#include<stdio.h>
#include<time.h>
#include<fstream>
#include<sstream>
#include<string>
#include<map>
#include<set>
using namespace std;

/* ------------------ USER ------------------ */
typedef struct history* adrHistory;
typedef struct user* adrUser;
struct history {
	string asal, tujuan;
	string waktuPesan, kode;
	int harga;
	adrHistory next;
};
struct user {
	int id;
	string username, password;
	adrHistory history;
	adrUser next;
};
adrHistory createHistory(string asal, string tujuan, string waktuPesan, string kode, int harga);
void addHistory(adrUser &orang, adrHistory history);
/* --------X--------- USER ---------X-------- */


/* ------------------ USER LINKED LIST ------------------- */
struct listUser {
	adrUser first;
};
adrUser createUser(string username, string password);
void setValUser(adrUser &user, int id, string username, string password);
void insertUser(listUser &L, adrUser orang);
void deleteUser(listUser &u, string username);
void printTiket(string asal, string tujuan, string waktu, string kode, int harga);
/* -----------X------ USER LINKED LIST ----------X-------- */

/* -------------------- GRAPH ------------------------ */
typedef struct Kota* adrKota;
typedef struct AdjKota* adrAdj;
struct Kota {
	int id;
	string namaKota;
	adrKota next;
	adrAdj firstAdj;
};
struct AdjKota {
	adrKota connectTo;
	int distance;
	adrAdj next;
};
struct Graph {  
	adrKota start;
	int banyakKota;
};
void createGraph(Graph &G);
void alocateKota(adrKota &N, string namaKota);
void alocateAdj(adrAdj &NA, adrKota connectTo, int distance);
void addNewKota(Graph &G, adrKota N);
adrKota getKota(Graph G, string N);
string getRandomCode();
void addKotaAdj(adrKota &N, adrAdj NA);
void connectKota(adrKota &N1, adrKota &N2, int dist);
void showKota(Graph G);
void delAdjKota(adrKota &N1, adrKota N2);
void disconnectKota(adrKota &N1, adrKota &N2);
void setValKota(adrKota &N, int id, string namaKota);
void printGraph(Graph G);
void delKota(Graph &G, adrKota N);
int jarakTerdekat(Graph G, adrKota n1, adrKota n2);
int calcPrice(Graph G, adrKota n1, adrKota n2);
/* -----------X-------- GRAPH ------------X----------- */
string getTime();
void clearScr();
void header();
void pause();
void menu(string title, string list[], int n);
#endif
