#include"header.h"

# define INF 0x3f3f3f3f 

/* ------------------ USER ------------------ */

adrHistory createHistory(string asal, string tujuan, string waktuPesan, string kode, int harga){
	adrHistory h = new history;
	h->asal = asal;
	h->tujuan = tujuan;
	h->waktuPesan = waktuPesan;
	h->kode = kode;
	h->harga = harga;
	h->next = NULL;
	return h;
}

void addHistory(adrUser &orang, adrHistory history){
	if (orang->history == NULL){
		orang->history = history;
	} else {
		history->next = orang->history;
		orang->history = history;
	}
}

/* --------X--------- USER ---------X-------- */


/* ------------------ USER LINKED LIST ------------------- */

adrUser createUser(string username, string password){
	adrUser u = new user;
	int id = rand()%89999 + 10000;
	u->id = id;
	u->username = username;
	u->password = password;
	u->history = NULL;
	u->next = NULL;
}

void setValUser(adrUser &User, int id, string username, string password){
	User = new user;
	User->id = id;
	User->username = username;
	User->password = password;
	User->history = NULL;
	User->next = NULL;
}

void insertUser(listUser &L, adrUser orang){
	if (L.first == NULL){
		L.first = orang;
	} else {
		orang->next = L.first;
		L.first = orang;
	}
}

void deleteUser(listUser &u, string username){
	adrUser current = u.first;
	while(current->next != NULL && current->next->username != username){
		current = current->next;
	}
	adrUser dummy = current->next;
	current->next = dummy->next;
	dummy->next = NULL;
}

void printTiket(string asal, string tujuan, string waktu, string kode, int harga){
	cout << "========================================\n";
	cout << "	         Tiket\n";
	cout << "========================================\n";
	cout << "    Kode        : " << kode << '\n';
	cout << "    Kota Asal   : " << asal << '\n';
	cout << "    Kota Tujuan : " << tujuan << '\n';
	cout << "    Waktu Pesan : " << waktu << '\n';
	cout << "    Harga       : " << harga << '\n';
	cout << "========================================\n";
}

/* -----------X------ USER LINKED LIST ----------X-------- */

/* -------------------- GRAPH ------------------------ */

void createGraph(Graph &G){
	G.start = NULL;
	G.banyakKota = 0;
}
void alocateKota(adrKota &N, string namaKota){
	N = new Kota;
	N->id = rand()%100 + 1;
	N->namaKota = namaKota;
	N->next = NULL;
	N->firstAdj = NULL;
}
void alocateAdj(adrAdj &NA, adrKota connectTo, int distance){
	NA = new AdjKota;
	NA->connectTo = connectTo;
	NA->distance = distance;
	NA->next = NULL;
}

void setValKota(adrKota &N, int id, string namaKota){
	N = new Kota;
	N->id = id;
	N->namaKota = namaKota;
	N->next = NULL;
	N->firstAdj = NULL;
}

void addNewKota(Graph &G, adrKota N){
	if (G.start == NULL){
		G.start = N;
	} else {
		adrKota current = G.start;
		while(current->next != NULL){
			current = current->next;
		}
		current->next = N;
	}
	G.banyakKota += 1;
}
adrKota getKota(Graph G, string nama){
	// mengembalikan Kota nya jika ketemu, atau NULL jika tidak
	adrKota current = G.start;
	while(current != NULL && current->namaKota != nama){
		current = current->next;
	}
	return current;
}
void addKotaAdj(adrKota &N, adrAdj NA){
	if (N->firstAdj == NULL){
		N->firstAdj = NA;
	} else {
		NA->next = N->firstAdj;
		N->firstAdj = NA;
	}
}
void connectKota(adrKota &N1, adrKota &N2, int dist){
	adrAdj n1AsChild, n2AsChild;
	alocateAdj(n1AsChild, N1, dist);
	alocateAdj(n2AsChild, N2, dist);
	addKotaAdj(N1, n2AsChild);
	addKotaAdj(N2, n1AsChild);
}

void delAdjKota(adrKota &N1, adrKota N2){
	adrAdj currentChild = N1->firstAdj;
	if (currentChild->connectTo == N2){
		N1->firstAdj = currentChild->next;
		currentChild->next = NULL;
	} else {
		while(currentChild->next != NULL && currentChild->next->connectTo != N2){
			currentChild = currentChild->next;
		}
		if (currentChild->next != NULL){
			adrAdj dummy = currentChild->next;
			currentChild->next = dummy->next;
			dummy->next = NULL;
		}
	}
}
void disconnectKota(adrKota &N1, adrKota &N2){
	delAdjKota(N1, N2);
	delAdjKota(N2, N1);
}

void delKota(Graph &G, adrKota N){
	adrKota current = G.start;
	// ngapus semua N sebagai child node lain
	while(current != NULL){
		delAdjKota(current, N);
		current = current->next;
	}
	// ngapus N pada list node
	current = G.start;
	if (current == N){
		G.start = N->next;
		N->next = NULL;
	} else {
		while(current->next != NULL && current->next != N){
			current = current->next;
		}
		if (current->next != NULL){
			current->next = N->next;
			N->next = NULL;
		}
	}
}

void showKota(Graph G){
	adrKota current = G.start;
	while(current != NULL){
		cout << current->namaKota;
		if(current->next != NULL) cout << ", ";
		current = current->next;
	}
	cout << '\n';
}

void printGraph(Graph G){
	adrKota currentKota = G.start;
	while(currentKota != NULL){
		cout << currentKota->namaKota << " : {";
		adrAdj currentAdj = currentKota->firstAdj;
		while(currentAdj != NULL){
			cout << "[" << currentAdj->connectTo->namaKota << "," << currentAdj->distance << "]";
			if (currentAdj->next != NULL) cout << ", ";
			currentAdj = currentAdj->next;
		}
		cout << "}\n";
		currentKota = currentKota->next;
	}
}

int jarakTerdekat(Graph G, adrKota from, adrKota to){
	// dijkstra algorithm, mencari jarakTerdekat/lintasanDgnBobotTerkecil antara 2 nodes
	map<adrKota, int> dist;
	set<pair<int, adrKota>> Q;
	
	adrKota current = G.start;
	while(current != NULL){
		dist[current] = INF;
		current = current->next;
	}
	dist[from] = 0;
	
	Q.insert(make_pair(0, from));
	
	while(!Q.empty()){
		pair<int, adrKota> tmp = *(Q.begin());
		Q.erase(Q.begin());
		adrKota u = tmp.second;
		
		for (adrAdj i = u->firstAdj; i != NULL; i = i->next){
			adrKota v = i->connectTo;
			int weight = i->distance;
			if (dist[v] > dist[u] + weight){
				if (dist[v] != INF)
					Q.erase(Q.find(make_pair(dist[v], v)));
				dist[v] = dist[u] + weight;
				Q.insert(make_pair(dist[v], v));
			}
		}
	}
	return dist[to];
}

int calcPrice(Graph G, adrKota n1, adrKota n2){
	const int tarifPerKm = 1000;
	return jarakTerdekat(G, n1, n2)*tarifPerKm;
}

/* -----------X-------- GRAPH ------------X----------- */

void clearScr(){
	system("CLS");
}

void header(){
	cout << "===================================================\n";
	cout << "          Aplikasi Pemesanan Tiket Kereta        \n";
	cout << "===================================================\n";
}

void pause(){
	cout << "Tekan apapun untuk melanjuti...";
	getch();
}

string getTime(){
	time_t rawtime;
	time(&rawtime);
	string s = ctime(&rawtime); // Tue Apr 28 14:08:41 2020    format
	// convert to 28-Apr-2020,14:08:41 format
	string theTime = s.substr(8,2) + '-' + s.substr(4,3) + '-' + s.substr(20,4) + ',' + s.substr(11,8); 
	return theTime;
}

string getRandomCode() {
    int len = 10;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    string s = "strOfLen10";
    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return s;
}

void menu(string title, string list[], int n){
	cout << ">> " << title << " : \n";
	for (int i = 1; i <= n; i++){
		printf("%d. ", i);
		cout << list[i-1] << "\n";
	}
	cout << "Pilih : ";
}
