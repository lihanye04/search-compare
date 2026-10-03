#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define NUM_DATA      100
#define NUM_SEARCH    50
#define MAX_VAL       1000

typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} Node;

Node *createNode(int data) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (n == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    n->data = data;
    n->left = NULL;
    n->right = NULL;
    return n;
}

int insertBST(Node **root, int data) {
    int comparisons = 0;
    Node *newNode = createNode(data);

    if (*root == NULL) {
        *root = newNode;
        return 0;
    }

    Node *curr = *root;
    Node *parent = NULL;

    while (curr != NULL) {
        comparisons++;
        parent = curr;

        if (data < curr->data) {
            curr = curr->left;
        } else if (data > curr->data) {
            curr = curr->right;
        } else {
            free(newNode);
            return comparisons;
        }
    }

    if (data < parent->data) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    return comparisons;
}

int sequentialSearch(int arr[], int n, int key, bool *found) {
    int comparisons = 0;
    for (int i = 0; i < n; i++) {
        comparisons++;
        if (arr[i] == key) {
            *found = true;
            return comparisons;
        }
    }
    *found = false;
    return comparisons;
}

int bstSearch(Node *root, int key, bool *found) {
    int comparisons = 0;
    Node *curr = root;
    while (curr != NULL) {
        comparisons++;
        if (key == curr->data) {
            *found = true;
            return comparisons;
        } else if (key < curr->data) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    *found = false;
    return comparisons;
}

void freeBST(Node *root) {
    if (root == NULL) return;
    freeBST(root->left);
    freeBST(root->right);
    free(root);
}

int main(void) {
    srand(12345);  // 固定随机种子，保证每次运行结果一致

    FILE *fresult = fopen("result.txt", "w");
    FILE *freadme = fopen("README.md", "w");
    if (!fresult || !freadme) {
        printf("File open error.\n");
        return 1;
    }

    int arr[NUM_DATA];
    bool used[MAX_VAL + 1] = {false};
    Node *root = NULL;
    int bstCreationComparisons = 0;

    int count = 0;
    while (count < NUM_DATA) {
        int val = rand() % (MAX_VAL + 1);
        if (!used[val]) {
            used[val] = true;
            arr[count] = val;
            bstCreationComparisons += insertBST(&root, val);
            count++;
        }
    }

    fprintf(fresult, "Generated 100 distinct integers:\n");
    for (int i = 0; i < NUM_DATA; i++) {
        fprintf(fresult, "%4d ", arr[i]);
        if ((i + 1) % 20 == 0) fprintf(fresult, "\n");
    }
    fprintf(fresult, "\n\n");
    fprintf(fresult, "BST Creation Total Comparisons: %d\n\n", bstCreationComparisons);

    int searchKeys[NUM_SEARCH];
    for (int i = 0; i < NUM_SEARCH; i++) {
        searchKeys[i] = rand() % (MAX_VAL + 1);
    }

    fprintf(fresult, "Generated 50 search keys:\n");
    for (int i = 0; i < NUM_SEARCH; i++) {
        fprintf(fresult, "%4d ", searchKeys[i]);
        if ((i + 1) % 20 == 0) fprintf(fresult, "\n");
    }
    fprintf(fresult, "\n\n");

    int totalSeqComp = 0;
    int totalBstComp = 0;

    fprintf(fresult, "Search Results:\n");
    for (int i = 0; i < NUM_SEARCH; i++) {
        int key = searchKeys[i];
        bool seqFound, bstFound;

        int seqComp = sequentialSearch(arr, NUM_DATA, key, &seqFound);
        int bstComp = bstSearch(root, key, &bstFound);

        totalSeqComp += seqComp;
        totalBstComp += bstComp;

        fprintf(fresult, "Search Key : %d\n", key);
        fprintf(fresult, "Sequential Search\n");
        fprintf(fresult, "Result      : %s\n", seqFound ? "Found" : "Not Found");
        fprintf(fresult, "Comparisons : %d\n", seqComp);
        fprintf(fresult, "BST Search\n");
        fprintf(fresult, "Result      : %s\n", bstFound ? "Found" : "Not Found");
        fprintf(fresult, "Comparisons : %d\n\n", bstComp);
    }

    fprintf(fresult, "Number of searches: %d\n\n", NUM_SEARCH);
    fprintf(fresult, "Sequential Search\n");
    fprintf(fresult, "Total comparisons   : %d\n", totalSeqComp);
    fprintf(fresult, "Average comparisons : %.2f\n\n", (double)totalSeqComp / NUM_SEARCH);
    fprintf(fresult, "BST Search\n");
    fprintf(fresult, "Total comparisons   : %d\n", totalBstComp);
    fprintf(fresult, "Average comparisons : %.2f\n\n", (double)totalBstComp / NUM_SEARCH);
    fprintf(fresult, "BST Creation\n");
    fprintf(fresult, "Total comparisons   : %d\n", bstCreationComparisons);

    fprintf(freadme, "# 순차 탐색과 이진 탐색 트리 탐색의 비교\n\n");
    fprintf(freadme, "## 1. 프로그램 설명\n\n");
    fprintf(freadme, "- 0부터 1,000 사이의 정수 100개를 임의 생성하여 배열과 이진 탐색 트리(BST)에 저장한다.\n");
    fprintf(freadme, "- 생성되는 100개의 정수는 서로 다른 값이다.\n");
    fprintf(freadme, "- 배열은 발생 순서 그대로 저장하고, BST는 같은 순서로 삽입한다.\n");
    fprintf(freadme, "- 0부터 1,000 사이의 정수 50개를 추가 생성하여 탐색 키로 사용한다.\n");
    fprintf(freadme, "- 각 탐색 키에 대해 배열에서는 순차 탐색, BST에서는 BST 탐색을 수행한다.\n");
    fprintf(freadme, "- 각 탐색에서 발생한 숫자 비교 횟수를 측정하고, BST 생성 시 발생한 비교 횟수도 함께 측정한다.\n\n");

    fprintf(freadme, "## 2. 실행 결과\n\n");
    fprintf(freadme, "### 2.1 생성된 100개의 서로 다른 정수\n\n");
    fprintf(freadme, "자세한 내용은 result.txt 참조.\n\n");
    fprintf(freadme, "### 2.2 BST 생성 과정 비교 횟수\n\n");
    fprintf(freadme, "- BST 생성 총 비교 횟수: **%d**\n\n", bstCreationComparisons);

    fprintf(freadme, "### 2.3 생성된 50개의 탐색 키\n\n");
    fprintf(freadme, "자세한 내용은 result.txt 참조.\n\n");
    fprintf(freadme, "### 2.4 각 탐색 키에 대한 결과\n\n");
    fprintf(freadme, "자세한 내용은 result.txt 참조.\n\n");

    fprintf(freadme, "### 2.5 통계\n\n");
    fprintf(freadme, "- 순차 탐색 총 비교 횟수: **%d**\n", totalSeqComp);
    fprintf(freadme, "- 순차 탐색 평균 비교 횟수: **%.2f**\n", (double)totalSeqComp / NUM_SEARCH);
    fprintf(freadme, "- BST 탐색 총 비교 횟수: **%d**\n", totalBstComp);
    fprintf(freadme, "- BST 탐색 평균 비교 횟수: **%.2f**\n", (double)totalBstComp / NUM_SEARCH);
    fprintf(freadme, "- BST 생성 총 비교 횟수: **%d**\n\n", bstCreationComparisons);

    fprintf(freadme, "## 3. 성능 분석\n\n");
    fprintf(freadme, "### 3.1 순차 탐색과 BST 탐색의 비교\n\n");
    fprintf(freadme, "순차 탐색은 배열의 첫 번째 원소부터 순서대로 비교하므로, 탐색 성공 시 평균적으로 약 N/2 번, 탐색 실패 시 N 번의 비교가 필요하다. 이번 실험에서 순차 탐색의 평균 비교 횟수는 **%.2f**회로 측정되었다.\n\n", (double)totalSeqComp / NUM_SEARCH);
    fprintf(freadme, "반면 BST 탐색은 트리의 높이에 비례하는 비교 횟수를 가진다. 무작위로 삽입된 BST는 평균적으로 O(log N)의 높이를 가지므로, 이번 실험에서 BST 탐색의 평균 비교 횟수는 **%.2f**회로 나타났다. 이는 순차 탐색보다 훨씬 적은 비교 횟수이다.\n\n", (double)totalBstComp / NUM_SEARCH);

    fprintf(freadme, "### 3.2 BST 생성 비용을 고려한 분석\n\n");
    fprintf(freadme, "BST는 탐색 성능이 좋지만, 생성 시 각 데이터를 삽입할 위치를 찾기 위한 비교 비용이 발생한다. 이번 실험에서 BST 생성에 **%d**회의 비교가 사용되었다.\n\n", bstCreationComparisons);
    fprintf(freadme, "50회 탐색에서 순차 탐색은 총 **%d**회, BST 탐색은 총 **%d**회의 비교를 사용했다. BST 생성 비용을 탐색 비용에 포함시키면, 총 비용은 BST가 **%d**회, 순차 탐색이 **%d**회가 된다.\n\n", totalSeqComp, totalBstComp, bstCreationComparisons + totalBstComp, totalSeqComp);
    fprintf(freadme, "만약 탐색 횟수가 많아지면 BST 생성 비용은 상대적으로 작아지고, BST 탐색의 성능 이점이 커진다. 반대로 탐색 횟수가 적으면 BST 생성 비용 때문에 순차 탐색이 더 유리할 수도 있다.\n\n");

    fprintf(freadme, "### 3.3 BST 구조에 따른 탐색 성능 변화\n\n");
    fprintf(freadme, "BST의 탐색 성능은 트리의 균형에 크게 의존한다. 데이터가 무작위로 삽입되면 평균적으로 O(log N)의 높이를 가지지만, 정렬된 순서로 삽입되면 한쪽으로 치우친 편향 트리가 되어 O(N)의 성능을 보인다.\n\n");
    fprintf(freadme, "이번 실험에서는 무작위로 데이터를 생성했기 때문에 비교적 균형 잡힌 트리가 만들어졌고, BST 탐색의 평균 비교 횟수가 **%.2f**회로 양호하게 나타났다. 만약 데이터가 정렬된 순서로 삽입되었다면 BST 탐색의 비교 횟수가 순차 탐색과 비슷하거나 더 나빠질 수 있다.\n\n", (double)totalBstComp / NUM_SEARCH);

    fprintf(freadme, "## 4. 결론\n\n");
    fprintf(freadme, "순차 탐색은 구현이 간단하지만 평균 비교 횟수가 데이터 개수에 비례한다. BST 탐색은 트리가 균형을 이룰 경우 평균 비교 횟수가 로그 스케일로 줄어들어 훨씬 효율적이다. 다만 BST는 생성 과정에서 추가적인 비교 비용이 발생하므로, 전체 성능을 평가할 때는 생성 비용까지 함께 고려해야 한다. 또한 BST의 성능은 삽입 순서에 따라 크게 달라질 수 있으므로, 균형 잡힌 트리를 유지하는 것이 중요하다.\n");

    fclose(fresult);
    fclose(freadme);
    freeBST(root);

    printf("result.txt and README.md have been generated successfully.\n");
    return 0;
}