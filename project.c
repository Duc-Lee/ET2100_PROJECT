#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

//------------Cau truc node danh sach ke theo DSLK----------//
struct node{
    int dinh;
    int trong_so;
    struct node *next; // luu vi tri node tiep theo
};
typedef struct node node;

// Ham tao node moi 
node *createnode(int dinh, int trong_so){/*  */
    // Cap phat dong cho node moi
    node *newnode = (node*)malloc(sizeof(node));
    // Gan cac gia tri cho node moi
    newnode->dinh = dinh;
    newnode->trong_so = trong_so;
    newnode->next = NULL;
    return newnode;
}
//---------------Cau truc do thi theo DSLK--------------//
struct Dothi{
    int so_dinh;
    // Luu mang danh sach ke
    node **danh_sach_ke;
};
typedef struct Dothi Dothi;

// Khoi tao Do thi 
Dothi *createDothi(int so_dinh){
    // Cap phat dong cho do thi
    Dothi *newgraph = (Dothi*)malloc(sizeof(Dothi));
    // so dinh cho do thi moi 
    newgraph->so_dinh = so_dinh;
    // Cap phat dong de tao ma tran ke bang mang 2 chieu
    newgraph->danh_sach_ke = (node**)malloc(so_dinh * sizeof(node*));
    // Khoi tao cac phan tu trong mang danh sach ke = NULL
    for(int i=0;i<so_dinh;i++){
        newgraph->danh_sach_ke[i] = NULL;
    }
    return newgraph;
}

// Them canh vao do thi vo huong 
void them_canh(Dothi *dothi, int dinh_nguon, int dinh_dich, int trong_so){
    // Them dinh dich vao danh sach ke cua dinh nguon
    node *newgraph = createnode(dinh_dich,trong_so);
    newgraph->next = dothi->danh_sach_ke[dinh_nguon];
    dothi->danh_sach_ke[dinh_nguon] = newgraph;
    // Them dinh nguon vao danh sach ke cua dinh dich
    newgraph = createnode(dinh_nguon,trong_so);
    newgraph->next = dothi->danh_sach_ke[dinh_dich];
    dothi->danh_sach_ke[dinh_dich] = newgraph;
}

//----------------Hang doi uu tien bang Min Heap-------------//
// Cau truc node luu dinh va khoang cach
struct node_min_heap{
    int dinh;
    int khoang_cach;
};
typedef struct node_min_heap node_min_heap;

// cau truc min heap
struct min_heap{
    int kich_thuoc;       // so luong phan tu hien tai
    int suc_chua;         // suc chua toi da
    int *vitri;           // vi tri dinh trong min heap
    node_min_heap **arr;  // mang tro den cac node trong heap
};
typedef struct min_heap min_heap;

// tao node min heap
node_min_heap *taoNodeMinHeap(int dinh, int khoang_cach) {
    node_min_heap *node = (node_min_heap*)malloc(sizeof(node_min_heap));
    node->dinh = dinh;
    node->khoang_cach = khoang_cach;
    return node;
}

// Tao min heap
min_heap *taoMinHeap(int suc_chua) {
    min_heap *minheap = (min_heap*)malloc(sizeof(min_heap));
    minheap->kich_thuoc = 0;
    minheap->suc_chua = suc_chua;
    minheap->vitri = (int*)malloc(suc_chua * sizeof(int));
    minheap->arr = (node_min_heap**)malloc(suc_chua * sizeof(node_min_heap*));
    return minheap;
}

// Hoan doi 
void swapNode(node_min_heap **a, node_min_heap **b) {
    node_min_heap *temp = *a;
    *a = *b;
    *b = temp;
}

// shift-down de duy tri tinh chat Min Heap
// Moi node cha phai nho hon hoac bang cac node con cua no
// node nho nhat luon o goc root
void shiftDown(min_heap *minheap, int idx) {
    int nho_nhat = idx;
    // tinh chi so con trai
    int trai = 2 * idx + 1;
    // Tinh chi so con phai
    int phai = 2 * idx + 2;

    if (trai < minheap->kich_thuoc &&
        minheap->arr[trai]->khoang_cach < minheap->arr[nho_nhat]->khoang_cach)
        nho_nhat = trai;

    if (phai < minheap->kich_thuoc &&
        minheap->arr[phai]->khoang_cach < minheap->arr[nho_nhat]->khoang_cach)
        nho_nhat = phai;
    
    // Nếu có 1 con nho hơn cha cần hoán đổi
    if (nho_nhat != idx) {
        // Cap nhat vi tri
        // Dinh moi dang o vi tri index
        minheap->vitri[minheap->arr[nho_nhat]->dinh] = idx;
        // Dinh moi dang o vi tri nho_nhat
        minheap->vitri[minheap->arr[idx]->dinh] = nho_nhat;

        // Thuc hien hoan doi
        swapNode(&minheap->arr[nho_nhat], &minheap->arr[idx]);
        // Goi lai de dua cay ve Min Heap ( cha nho hon con)
        shiftDown(minheap, nho_nhat);
    }
}

// Kiem tra Heap co rong khong
int isEmpty(min_heap *minheap) {
    return minheap->kich_thuoc == 0;
}

// Lay node co khoang cach nho nhat va loại bo no khoi Heap
node_min_heap *extractMin(min_heap *minheap) {
    // Neu danh sach rong thi dung ham
    if (isEmpty(minheap))
        return NULL;
    
    node_min_heap *root = minheap->arr[0];
    node_min_heap *cuoi = minheap->arr[minheap->kich_thuoc - 1];
    minheap->arr[0] = cuoi;

    // Cap nhat vi tri 
    minheap->vitri[root->dinh] = minheap->kich_thuoc - 1;
    minheap->vitri[cuoi->dinh] = 0;

    minheap->kich_thuoc--;
    shiftDown(minheap, 0);
    return root;
}

// Giảm giá trị khoảng cách của một đỉnh và shift-up để duy trì tính chất min heap
void decreaseKey(min_heap *minheap, int dinh, int khoang_cach) {
    int i = minheap->vitri[dinh];
    minheap->arr[i]->khoang_cach = khoang_cach;

    while (i && minheap->arr[i]->khoang_cach < minheap->arr[(i - 1) / 2]->khoang_cach) {
        // Cap nhat vi tri
        minheap->vitri[minheap->arr[i]->dinh] = (i - 1) / 2;
        minheap->vitri[minheap->arr[(i - 1) / 2]->dinh] = i;

        // Hoan doi
        swapNode(&minheap->arr[i], &minheap->arr[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Kiem tra dinh con trong Min Heap khong
int isInMinHeap(min_heap *minheap, int dinh) {
    return minheap->vitri[dinh] < minheap->kich_thuoc;
}

// Hàm giải phóng bộ nhớ của MinHeap
void giaiPhongMinHeap(min_heap* minHeap) {
    if (minHeap == NULL) return;

    for (int i = 0; i < minHeap->kich_thuoc; i++) {
        if (minHeap->arr[i] != NULL) {
            free(minHeap->arr[i]);
        }
    }
    free(minHeap->arr);
    free(minHeap->vitri);
    free(minHeap);
}

//----------------Thuat toan Dijkstra--------------//
// Thuật toán Dijkstra tìm đường đi ngắn nhất và lưu parent
void dijkstra(Dothi* doThi, int dinhNguon, int khoangCach[], int parent[]) {
    int soDinh = doThi->so_dinh;
    min_heap* minHeap = taoMinHeap(soDinh);

    // Khởi tạo khoảng cách và cha cho mỗi đỉnh
    for (int v = 0; v < soDinh; v++) {
        khoangCach[v] = INT_MAX;
        parent[v] = -1;
        minHeap->arr[v] = taoNodeMinHeap(v, khoangCach[v]);
        minHeap->vitri[v] = v;
    }

    // Khoảng cách từ nguồn đến chính nó là 0
    khoangCach[dinhNguon] = 0;
    decreaseKey(minHeap, dinhNguon, khoangCach[dinhNguon]);
    minHeap->kich_thuoc = soDinh;

    // Lặp cho đến khi heap rỗng
    while (!isEmpty(minHeap)) {
        // Lấy đỉnh có khoảng cách nhỏ nhất
        node_min_heap* nodeMin = extractMin(minHeap);
        int u = nodeMin->dinh;
        free(nodeMin); // Giải phóng bộ nhớ

        // Duyệt các đỉnh kề của u
        node* ke = doThi->danh_sach_ke[u];
        while (ke != NULL) {
            int v = ke->dinh;

            // Cập nhật khoảng cách nếu tìm được đường đi ngắn hơn
            if (isInMinHeap(minHeap, v) && khoangCach[u] != INT_MAX &&
                khoangCach[u] + ke->trong_so < khoangCach[v]) {
                khoangCach[v] = khoangCach[u] + ke->trong_so;
                parent[v] = u;
                decreaseKey(minHeap, v, khoangCach[v]);
            }
            ke = ke->next;
        }
    }

    giaiPhongMinHeap(minHeap); // Giải phóng bộ nhớ heap
}



// In danh sach dinh ke 
void inDanhSachKe(Dothi *dothi, char *tenToaNha[]) {
    for (int i = 0; i < dothi->so_dinh; i++) {
        printf("%s: ", tenToaNha[i]);
        node *head = dothi->danh_sach_ke[i];
        while (head != NULL) {
            printf("-> %s (w=%d) ", tenToaNha[head->dinh], head->trong_so);
            head = head->next;
        }
        printf("\n");
    }
}

//-------------------- Giai phong vung nho------------//
// Hàm giải phóng bộ nhớ của đồ thị
void giaiPhongDoThi(Dothi* doThi) {
    if (doThi == NULL) return;

    for (int i = 0; i < doThi->so_dinh; i++) {
        node* hienTai = doThi->danh_sach_ke[i];
        while (hienTai != NULL) {
            node* temp = hienTai;
            hienTai = hienTai->next;
            free(temp);
        }
    }
    free(doThi->danh_sach_ke);
    free(doThi);
}

//--------- Thong tin khoang cach giua cac toa----------//
typedef struct {
    const char *toa1;
    const char *toa2;
    int khoang_cach; // mét
} Canh;

Canh ds_canh[] = {
    {"D3", "D3-5", 20},
    {"D3-5", "B1", 100},
    {"B1", "TC", 200},
    {"D3", "D7", 100},
    {"D3", "ThuVien", 40},
    {"ThuVien", "D7", 20},
    {"D3-5", "D7", 90}
};
int so_canh = sizeof(ds_canh) / sizeof(Canh);

int lay_khoang_cach(const char *nguon, const char *dich) {
    for (int i = 0; i < so_canh; i++) {
        if ((strcmp(ds_canh[i].toa1, nguon) == 0 && strcmp(ds_canh[i].toa2, dich) == 0) ||
            (strcmp(ds_canh[i].toa2, nguon) == 0 && strcmp(ds_canh[i].toa1, dich) == 0)) {
            return ds_canh[i].khoang_cach;
        }
    }
    return -1; // Khong tim thay 
}

// Thêm hàm timIndex để không lỗi khi gọi
int timIndex(char* tenToaNha[], int soDinh, const char* toa) {
    for (int i = 0; i < soDinh; i++) {
        if (strcmp(tenToaNha[i], toa) == 0) {
            return i;
        }
    }
    return -1; // Khong tim thay
}

/// ---------- In duong di bat ki --------------///

// Hàm in đường đi (đệ quy)
void inDuongDi(int parent[], int j, char* tenToaNha[]) {
    if (parent[j] == -1) {
        printf("%s", tenToaNha[j]);
        return;
    }
    inDuongDi(parent, parent[j], tenToaNha);
    printf(" -> %s", tenToaNha[j]);
}

// Tim duong di 
int timduong(Dothi *dothi, int nguon, int dich, int *parent, int *visited) {
    visited[nguon] = 1;  // danh dau da tham
    node *ke = dothi->danh_sach_ke[nguon];
    while(ke != NULL) {
        int v = ke->dinh;
        if(!visited[v]) {  // neu chua tham
            // luu dinh cha
            parent[v] = nguon;  
            if(v == dich || timduong(dothi, v, dich, parent, visited)) {
                // Neu tim thay duong thi tra ve 1
                return 1;  
            }
        }
        ke = ke->next;  
    }
    return 0;  
}

void induongdibatki(Dothi *dothi, char *tenToaNha[], int soDinh) {
    char nguon[20];
    char dich[20];
    printf("Nhap toa nha nguon: ");
    scanf("%s", nguon);
    printf("Nhap toa nha dich: ");
    scanf("%s", dich);
    
    // Tìm chỉ số tương ứng trong danh sách tên
    int dinhNguon = timIndex(tenToaNha, soDinh, nguon);
    int dinhDich = timIndex(tenToaNha, soDinh, dich);
    
    // Check out put
    if(dinhNguon == -1 || dinhDich == -1) {
        printf("Toa nha khong ton tai trong danh sach!\n");
        return;
    }

    int *visited = (int*)calloc(soDinh, sizeof(int));
    int *parent = (int*)malloc(soDinh * sizeof(int));
    
    // Danh dau cac dinh bag -1 het la chua tham
    for(int i = 0; i < soDinh; i++) {
        parent[i] = -1;
    }
    
    if(timduong(dothi, dinhNguon, dinhDich, parent, visited)) {
        printf("Duong di bat ki: ");
        inDuongDi(parent, dinhDich, tenToaNha);
        printf("\n");
    } else {
        printf("Khong tim thay duong di!\n");
    }
    free(visited);
    free(parent);
}

int main() {
    // Danh sách tên các tòa nhà
    char *tenToaNha[] = {
        "D3", "D3-5", "D7", "B1", "TC", "ThuVien"
    };
    int soDinh = sizeof(tenToaNha) / sizeof(tenToaNha[0]);

    // Khởi tạo đồ thị
    Dothi *doThi = createDothi(soDinh);
    char nguon[20], dich[20];
    int lc1;

    while (1) {
        // Menu chính
        printf("---- Duong di ngan nhat -------\n");
        printf("1. Nhap vi tri cac toa\n");
        printf("2. In danh sach dinh ke\n");
        printf("3. In duong di bat ki\n");
        printf("0. Ket thuc chuong trinh\n");
        printf("Chon: ");
        scanf("%d", &lc1);

        if (lc1 == 1) {
            // Thêm các cạnh từ danh sách ds_canh
            for (int i = 0; i < so_canh; i++) {
                int u = timIndex(tenToaNha, soDinh, ds_canh[i].toa1);
                int v = timIndex(tenToaNha, soDinh, ds_canh[i].toa2);
                int trong_so = ds_canh[i].khoang_cach;

                if (u != -1 && v != -1) {
                    them_canh(doThi, u, v, trong_so);
                }
            }

            // Nhập tên đỉnh nguồn và đỉnh đích
            printf("Nhap ten dinh nguon: ");
            scanf("%s", nguon);
            printf("Nhap ten dinh dich: ");
            scanf("%s", dich);

            // Tìm chỉ số tương ứng trong danh sách tên
            int dinhNguon = timIndex(tenToaNha, soDinh, nguon);
            int dinhDich = timIndex(tenToaNha, soDinh, dich);

            if (dinhNguon == -1 || dinhDich == -1) {
                printf("Khong tim thay dinh nguon hoac dinh dich!\n");
                giaiPhongDoThi(doThi);
                return 1;
            }

            // Cấp phát mảng khoảng cách và cha
            int *khoangCach = (int *)malloc(soDinh * sizeof(int));
            int *parent = (int *)malloc(soDinh * sizeof(int));

            // Chạy thuật toán Dijkstra
            dijkstra(doThi, dinhNguon, khoangCach, parent);

            if (khoangCach[dinhDich] == INT_MAX) {
                printf("Khong co duong di tu %s den %s\n", nguon, dich);
            } else {
                // In đường đi và khoảng cách ngắn nhất
                printf("Duong di ngan nhat tu %s den %s la: ", nguon, dich);
                inDuongDi(parent, dinhDich, tenToaNha);
                printf("\nTong khoang cach: %d met\n", khoangCach[dinhDich]);

                // Nếu nguồn hoặc đích có trong danh sách tên thì gọi Python vẽ hình
                int canRunPython = 0;
                for (int i = 0; i < soDinh; i++) {
                    if (strcmp(nguon, tenToaNha[i]) == 0 || strcmp(dich, tenToaNha[i]) == 0) {
                        canRunPython = 1;
                        break;
                    }
                }

                // Gọi file Python để hiển thị hình ảnh minh họa đường đi
                if (canRunPython) {
                    char command[200];
                    sprintf(command, "python picture.py %s %s", nguon, dich);
                    system(command);
                }
            }

            free(khoangCach);
            free(parent);
            getchar();
        } 
        else if (lc1 == 2) {
            printf("\nDANH SACH KE CUA DO THI:\n");
            inDanhSachKe(doThi, tenToaNha);
            printf("\n");
        }
        else if(lc1 == 3){
            // Thêm các cạnh từ danh sách ds_canh
            for (int i = 0; i < so_canh; i++) {
                int u = timIndex(tenToaNha, soDinh, ds_canh[i].toa1);
                int v = timIndex(tenToaNha, soDinh, ds_canh[i].toa2);
                int trong_so = ds_canh[i].khoang_cach;

                if (u != -1 && v != -1) {
                    them_canh(doThi, u, v, trong_so);
                }
            }
           induongdibatki(doThi,tenToaNha,soDinh);
        }
        else if (lc1 == 0) {
            break; // Thoát chương trình
        } 
        else {
            printf("Vui long nhap lai!\n");
        }
    }

    giaiPhongDoThi(doThi); // Giải phóng bộ nhớ đồ thị
    return 0;
}
