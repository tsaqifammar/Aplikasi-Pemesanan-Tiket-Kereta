#include"header.h"

/* ---------------- GLOBAL VARIABLES ----------------- */

// User linked list, Graph pemetaan kota, 
listUser usersDB;
adrUser userMakai;
Graph G;

/* -------X-------- GLOBAL VARIABLES ---------X------- */

void initialize(){
	// insert data from files to program
	
	srand(time(NULL)); // generate random seed
	usersDB.first = NULL;
	
	// masukin semua data ke usersDB
	ifstream fileUsers;
	fileUsers.open("users.txt", ios::in);
	string s;
	while(getline(fileUsers, s)){
		// buat new User
		adrUser temp;
		istringstream iss (s);
		int id;
		string username, password;
		iss >> id >> username >> password;
		setValUser(temp, id, username, password);
		
		// masukin history per user
		string asal, tujuan, waktu, kode;
		int harga;
		while(iss >> asal >> tujuan >> waktu >> kode >> harga){
			adrHistory tempHis = createHistory(asal, tujuan, waktu, kode, harga);
			addHistory(temp, tempHis);
		}
		// masukin User filled with infos to the linked list
		insertUser(usersDB, temp);
	}
	fileUsers.close();
	
	// inisialisasi graf
	ifstream fileGraph;
	fileGraph.open("graph.txt", ios::in);
	createGraph(G);
	// buat nodes
	string temp;
	getline(fileGraph, temp);
	int n = stoi(temp);
	
	for (int i = 1; i <= n; i++){
		string city;
		getline(fileGraph, city);
		// masukin ke graph
		adrKota temp;
		alocateKota(temp, city);
		addNewKota(G, temp); 
	}
	// masukin adjs ke tiap node
	string adjacents;
	adrKota current = G.start;
	while(getline(fileGraph, adjacents)){
		istringstream iss2 (adjacents);
		string connectTo;
		int distance;
		while(iss2 >> connectTo >> distance){
			// alocate adj, trus masukin ke current 
			adrKota connect = getKota(G, connectTo);
			adrAdj tempAdj;
			alocateAdj(tempAdj, connect, distance);
			addKotaAdj(current, tempAdj);
		}
		current = current->next;
	}
}

void registerUser(){
	clearScr(); header();
	string nama, password;
	cout << ">> REGISTER\n";
	cout << "Masukkan username : ";
	cin >> nama;
	cout << "Masukkan password : ";
	cin >> password;
	cout << "Anda sudah terdaftar!\n";
	adrUser pengguna = createUser(nama, password);
	insertUser(usersDB, pengguna);
	pause();
}

void pesanTiket(){
	clearScr(); header();
	string asal, tujuan;
	cout << "Kota yang ada : ";
	showKota(G);
	cout << "Masukkan kota asal : ";
	cin >> asal;
	cout << "Masukkan kota tujuan : ";
	cin >> tujuan;
	adrKota nodeAsal = getKota(G, asal), nodeTujuan = getKota(G, tujuan);
	if (nodeAsal != NULL && nodeTujuan != NULL){
		string waktu = getTime(), kode = getRandomCode();
		int harga = calcPrice(G, nodeAsal, nodeTujuan);
		adrHistory temp = createHistory(asal, tujuan, waktu, kode, harga);
		printTiket(asal, tujuan, waktu, kode, harga);
		char confirm;
		cout << "Konfirmasi Pembelian (Y/N) : ";
		cin >> confirm;
		if (confirm == 'Y'){
			addHistory(userMakai, temp);
			cout << "Pemesanan tiket berhasil.\n";
			pause();
		}	
	} else {
		cout << "Asal atau tujuan tidak ditemukan\n";
		pause();
	}
}

void showRiwayat(){
	clearScr(); header();
	int counter = 1;
	cout << "Riwayat Pemesanan : \n";
	adrHistory current = userMakai->history;
	while(current != NULL){
		cout << counter << ".\n";
		printTiket(current->asal, current->tujuan, current->waktuPesan, current->kode, current->harga);
		current = current->next;
		counter += 1;
	}
	pause();
}

void pengaturanAkun(){
	AKUN:
	clearScr(); header();
	string arr[] = {"Ganti Nama", "Ganti Password", "Kembali"};
	menu("PENGATURAN AKUN", arr, 3);
	int pilih;
	cin >> pilih;
	clearScr(); header();
	switch(pilih){
		case 1:{
			cout << "Masukkan nama baru : ";
			string nama;
			cin >> nama;
			userMakai->username = nama;
			cout << "Nama anda sudah terupdate!\n";
			pause();
			break;
		}
		case 2:{
			cout << "Masukkan password baru : ";
			string password;
			cin >> password;
			userMakai->password = password;
			cout << "Password anda sudah terupdate!\n";
			pause();
			break;
		}
		case 3:{
			goto KELUAR;
			break;
		}
		default:{
			goto AKUN;
			break;
		}
	}
	goto AKUN;
	KELUAR:{}
}

void pemetaanKota(){
	KOTA:
	clearScr(); header();
	string arr[] = {"Daftar Kota dan Hubungannya", "Tambah Kota", "Hubungkan Kota", "Hapus kota", "Kembali"};
	menu("PEMETAAN KOTA", arr, 5);
	int pilih;
	cin >> pilih;
	switch(pilih){
		case 1:{
			printGraph(G);
			pause();
			break;
		}
		case 2:{
			adrKota kotaBaru;
			string namaKota;
			cout << "Masukkan nama kota baru : ";
			cin >> namaKota;
			alocateKota(kotaBaru, namaKota);
			addNewKota(G, kotaBaru);
			pause();
			break;
		}
		case 3:{
			string namaKota1, namaKota2;
			int jarak;
			cout << "Masukkan nama Kota 1 : ";
			cin >> namaKota1;
			cout << "Masukkan nama Kota 2 : ";
			cin >> namaKota2;
			cout << "Masukkan jarak antara kedua kota tersebut : ";
			cin >> jarak;
			adrKota kota1 = getKota(G, namaKota1);
			adrKota kota2 = getKota(G, namaKota2);
			if (kota1 != NULL && kota2 != NULL){
				connectKota(kota1, kota2, jarak);
			} else {
				cout << "Salah satu kota tidak ditemukan\n";
				pause();
			}
			break;
		}
		case 4:{
			string kotaHapus;
			cout << "Masukkan kota yang ingin dihapus : ";
			cin >> kotaHapus;
			adrKota hapusCity = getKota(G, kotaHapus);
			if (hapusCity != NULL){
				delKota(G, hapusCity);
			} else {
				cout << "Tidak ditemukan kota bernama tersebut\n";
				pause();
			}
			break;
		}
		case 5:{
			goto KELUAR;
			break;
		}
		default:{
			goto KOTA;
			break;
		}
	}
	goto KOTA;
	KELUAR:{}
}

void home(bool admin){
	HOME:
	clearScr(); header();
	if (!admin){
		string arr[] = {"Pesan Tiket Kereta", "Riwayat Pemesanan", "Pengaturan Akun", "Keluar"};
		menu("HOME", arr, 4);
	} else {
		string arr[] = {"Pesan Tiket Kereta", "Riwayat Pemesanan", "Pengaturan Akun", "Pemetaan Kota", "Keluar"};
		menu("HOME", arr, 5);
	}
	int pilih;
	cin >> pilih;
	switch(pilih){
		case 1:{
			pesanTiket();
			break;
		}
		case 2:{
			showRiwayat();
			break;
		}
		case 3:{
			pengaturanAkun();
			break;
		}
		case 4:{
			if (admin) 
				pemetaanKota();
			else 
				goto OUT;
			break;
		}
		case 5:{
			goto OUT;
			break;
		}
		default: {
			goto HOME;
			break;
		}
	}
	goto HOME;
	OUT:{}
}


void loginUser(){
	LOGIN:
	clearScr(); header();
	string nama, password;
	cout << ">> LOGIN\n";
	cout << "Masukkan username : ";
	cin >> nama;
	cout << "Masukkan password : ";
	cin >> password;
	// cari
	adrUser current = usersDB.first;
	while(current != NULL){
		if (current->username == nama && current->password == password) break;
		current = current->next;
	}
	if (current == NULL){
		cout << "Tidak ditemukan user dengan username dan password tersebut\n";
		pause();
	} else {
		bool admin = (current->username == "admin123" && current->password == "admin123");
		userMakai = current;
		home(admin);
	}
}

void tulisLinkedListBarukeFile(){
	ofstream fileUsers;
	fileUsers.open("users.txt", ofstream::out | ofstream::trunc);
	adrUser current = usersDB.first;
	while(current != NULL){
		fileUsers << current->id << ' ' << current->username << ' ' << current->password;
		adrHistory curHist = current->history;
		while(curHist != NULL){
			fileUsers << ' ' << curHist->asal << ' ' << curHist->tujuan << ' ' << curHist->waktuPesan << ' ' << curHist->kode << ' ' << curHist->harga;
			curHist = curHist->next;
		}
		fileUsers << '\n';
		current = current->next;
	}
	fileUsers.close();
}

void tulisGraphBarukeFile(){
	ofstream fileGraph;
	fileGraph.open("graph.txt", ofstream::out | ofstream::trunc);
	fileGraph << G.banyakKota << '\n';
	adrKota current = G.start;
	// nodes
	while(current != NULL){
		fileGraph << current->namaKota << '\n';
		current = current->next;
	}
	// adjacents
	current = G.start;
	while(current != NULL){
		adrAdj currAdj = current->firstAdj;
		while(currAdj != NULL){
			fileGraph << currAdj->connectTo->namaKota << ' ' << currAdj->distance << ' ';
			currAdj = currAdj->next;
		}
		fileGraph << '\n';
		current = current->next;
	}
}

int main() {
	cout << "Loading... \n";
	initialize();
	MULAI:
	clearScr(); header();
	
	string arr[] = {"Register", "Login", "Keluar"};
	menu("MENU", arr, 3);
	
	int pilih1;
	cin >> pilih1;
	switch(pilih1){
		case 1:{
			registerUser();
			break;
		}
		case 2:{
			loginUser();
			break;
		}
		case 3:{
			cout << "Terimakasih telah menggunakan :)\n";
			goto KELUAR;
			break;
		}
		default:{
			goto MULAI;
			break;
		}
	}
	goto MULAI;
	KELUAR:
	tulisLinkedListBarukeFile();
	tulisGraphBarukeFile();
	exit(0);
	return 0;
}


