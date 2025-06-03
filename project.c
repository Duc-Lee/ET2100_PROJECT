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
node *createnode(int dinh, int trong_so){
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
    node_min_heap **arr;  // mang 2 chieu 
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

    if (nho_nhat != idx) {
        // Cap nhat vi tri
        minheap->vitri[minheap->arr[nho_nhat]->dinh] = idx;
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

    for (int v = 0; v < soDinh; v++) {
        khoangCach[v] = INT_MAX;
        parent[v] = -1;
        minHeap->arr[v] = taoNodeMinHeap(v, khoangCach[v]);
        minHeap->vitri[v] = v;
    }
    khoangCach[dinhNguon] = 0;
    decreaseKey(minHeap, dinhNguon, khoangCach[dinhNguon]);
    minHeap->kich_thuoc = soDinh;

    while (!isEmpty(minHeap)) {
        node_min_heap* nodeMin = extractMin(minHeap);
        int u = nodeMin->dinh;
        free(nodeMin); // Giải phóng node sau khi lấy ra khỏi heap

        node* ke = doThi->danh_sach_ke[u];
        while (ke != NULL) {
            int v = ke->dinh;
            if (isInMinHeap(minHeap, v) && khoangCach[u] != INT_MAX &&
                ke->trong_so + khoangCach[u] < khoangCach[v]) {
                khoangCach[v] = khoangCach[u] + ke->trong_so;
                parent[v] = u; // Cập nhật đỉnh cha của v là u
                decreaseKey(minHeap, v, khoangCach[v]);
            }
            ke = ke->next;
        }
    }

    giaiPhongMinHeap(minHeap); // Giải phóng MinHeap sau khi sử dụng xong
}

// Hàm in đường đi (đệ quy)
void inDuongDi(int parent[], int j, char* tenToaNha[]) {
    if (parent[j] == -1) {
        printf("%s", tenToaNha[j]);
        return;
    }
    inDuongDi(parent, parent[j], tenToaNha);
    printf(" -> %s", tenToaNha[j]);
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

int main() {
    char *tenToaNha[] = {
        "D3", "D3-5", "D5", "D7", "D9", "B1", "C7", "C2", "TC", "B3", "ThuVien"
    };
    int soDinh = sizeof(tenToaNha) / sizeof(tenToaNha[0]);
    
    Dothi *doThi = createDothi(soDinh);
    char nguon[20], dich[20];
    int lc1;

    
    while(1){
        printf("---- Duong di ngan nhat -------\n");
        printf("1. Nhap vi tri cac toa\n");
        printf("0. Ket thuc chuong trinh\n");
        printf("Chon: ");
        scanf("%d", &lc1);

        if(lc1 == 1 ){
            // Thêm các cạnh vào đồ thị dựa trên ds_canh
            for (int i = 0; i < so_canh; i++) {
                int u = timIndex(tenToaNha, soDinh, ds_canh[i].toa1);
                int v = timIndex(tenToaNha, soDinh, ds_canh[i].toa2);
                int trong_so = ds_canh[i].khoang_cach;

                if (u != -1 && v != -1) {
                    them_canh(doThi, u, v, trong_so);
                }
            }

            printf("Nhap ten dinh nguon: ");
            scanf("%s", nguon);
            printf("Nhap ten dinh dich: ");
            scanf("%s", dich);

            int dinhNguon = timIndex(tenToaNha, soDinh, nguon);
            int dinhDich = timIndex(tenToaNha, soDinh, dich);

            if (dinhNguon == -1 || dinhDich == -1) {
                printf("Khong tim thay dinh nguon hoac dinh dich!\n");
                giaiPhongDoThi(doThi);
                return 1;
            }

            int *khoangCach = (int *)malloc(soDinh * sizeof(int));
            int *parent = (int *)malloc(soDinh * sizeof(int));

            dijkstra(doThi, dinhNguon, khoangCach, parent);

            if (khoangCach[dinhDich] == INT_MAX) {
                printf("Khong co duong di tu %s den %s\n", nguon, dich);
            } else {
                printf("Duong di ngan nhat tu %s den %s la: ", nguon, dich);
                inDuongDi(parent, dinhDich, tenToaNha);
                printf("\nTong khoang cach: %d met\n", khoangCach[dinhDich]);
                if(strcmp(nguon,"TC") == 0 && strcmp(dich,"D3") == 0){
                    system("start TCdenD3.png");
                } 
                // Hút sạch ký tự newline còn sót lại trong bộ đệm trước khi đợi Enter
                char c;
                while ((c = getchar()) != '\n' && c != EOF);
                    printf("Nhan Enter de dong chuong trinh...\n");
                getchar();  // Nhan enter de dung
            }

            free(khoangCach);
            free(parent);
            // Giai phong vung nho de bat dau nhap do thi lan nua
        }
        else if( lc1 != 0 || lc1 != 1){
            printf("Vui long nhap lai!\n");
        }
        else if(lc1 == 0){
            break;  
        }
    }
    giaiPhongDoThi(doThi); // Giải phóng đồ thị khi thoát chương trình
}
       

   