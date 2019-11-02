#include<stdio.h>
#include<malloc.h>
#define MAX 100
int m[MAX];
int n[MAX];
int s[MAX];
int p=0;
int q=0;
int t=0;
typedef struct Node//平衡二叉树的结构体
{
    int data;
    struct Node *left;
    struct Node *right;
    int height;  //以此节点为根，树的高度；
    unsigned int freq;//此节点保存的数据出现的频率
}Node,*NODE;

int NodeHeight(NODE  ptrTree)//空树的高度-1
{
    return ptrTree==NULL ? -1 :ptrTree->height;
}
int max(int a,int b)
{
    return a<b ? b : a;
}

NODE LL(NODE k1)//LL旋转
{
    NODE k= k1->left;  //保存节点的左子树
    k1->left = k->right;   //k节点的右子树作为k1的左子树
    k->right =k1;          //把k1的k的右子树
    k1->height= max(NodeHeight(k1->left),NodeHeight(k1->right))+1;
    k->height= max(NodeHeight(k->left),NodeHeight(k->right))+1;
    return k;
}

NODE RR(NODE k2)
{
    NODE k= k2->right;  //保存节点的右子树
    k2->right=k->left;
    k->left=k2;
    k2->height= max(NodeHeight(k2->left),NodeHeight(k2->right))+1;
    k->height= max(NodeHeight(k->left),NodeHeight(k->right))+1;
    return k;
}

NODE LR(NODE k3)
{
    k3=RR(k3->left);
    k3=LL(k3);
    return k3;
}

NODE RL(NODE k3)
{
    k3=LL(k3->right);
    k3=RR(k3);
    return k3;
}

NODE LeftBalance(NODE node)//左平衡处理
{
    NODE ptrTmp=node->left;
    if(NodeHeight(ptrTmp->left)-NodeHeight(ptrTmp->right)==-1)//右子树高于左子树，在右子树插入的
        node=LR(node); //LR
    else
        node=LL(node); //LL
    return node;
}

NODE RightBalance(NODE node)//右平衡处理
{
    NODE ptrTmp=node->right;
    if(NodeHeight(ptrTmp->right)-NodeHeight(ptrTmp->left)==-1) //左子树比右子树高，说明在左子树插入的
        node=RL(node); //RL
    else
        node=RR(node);  //RR
}

NODE AVL_Insert(NODE node,int x)
{

    if(NULL==node) //找到插入的节点的位置
    {
        node =(NODE)malloc(sizeof(Node));
        node->data=x;
        node->height=0;
        node->freq = 1;
        node->left=NULL;
        node->right=NULL;
       // printf("~%d",node->data);
    }
    else if(x<node->data)  //在左子树插入
    {
       // printf("lll");
        node->left=AVL_Insert(node->left,x);
        if (NodeHeight(node->left)-NodeHeight(node->right)==2)//判断是否破坏平衡
            node=LeftBalance(node);  //左平衡处理
    }
    else if(node->data<x)   //在右子树插入
    {
        node->right=AVL_Insert(node->right,x);
        if (NodeHeight(node->right)-NodeHeight(node->left)==2)//判断是否破坏平衡
            node=RightBalance(node); //右平衡处理

    }
    else
        node->freq++;

    node->height = max(NodeHeight(node->left),NodeHeight(node->right)) +1 ;  //更新树的高度
    return node;
}

NODE AVL_Find(NODE node,int x)
{
    if (node==NULL)  //没找到元素
    {
        return NULL;
    }
    else if(x<node->data)
    {
         AVL_Find(node->left,x); //在左子树里面查找
    }
    else if(node->data<x)
    {
         AVL_Find(node->right,x); //在右子树里面查找
    }
    else //相等
        return node;
}

NODE AVL_Delete(NODE node,int x)
{
    if(NULL==node)  //空树直接返回
        return;
    if(x<node->data)  //在左子树中查找
    {
         node->left=AVL_Delete(node->left,x);
         if(NodeHeight(node->right)-NodeHeight(node->left)==2)  //树左平衡处理
          node=RightBalance(node);
    }
    else if(node->data<x)  //在右子树中查找
    {
        node->right=AVL_Delete(node->right,x);
         if(NodeHeight(node->left)-NodeHeight(node->right)==2)  //树右平衡处理
          node=LeftBalance(node);
    }
    else //找到要删除的元素节点
    {
        if(node->left==NULL) //左子树为空
        {
            NODE ptrTmp = node;
            node=node->right;         //用右孩子代替此节点
            free(ptrTmp);            //释放内存
        }
        else if(node->right==NULL)  //右子树为空
        {
            NODE ptrTmp = node;
            node=node->left;       //用左孩子代替此节点
            free(ptrTmp);
        }
        else   //左右子树都不为空
        {
            //用左子树的最小数据或右子树的最小数据代替该节点
            NODE ptrTmp=node->left;  //从左子树中查找
            while(ptrTmp->right!=NULL)
                ptrTmp=ptrTmp->right;//ptrTmp指向左子树中的最大元素
            node->data =  ptrTmp->data;
            node->left=AVL_Delete(node->left,ptrTmp->data);  //递归的删除该节点
        }
    }

    //更新节点的高度
    if(node)
        node->height = max(NodeHeight(node->left),NodeHeight(node->right));

    return node;
}

//中序遍历
void print(NODE node)
{
    if (NULL == node)
    {
        return ;
    }
    print(node->left);
    printf("%d ",node->data);
    print(node->right);
}

int main()
{
    NODE root;
    root=NULL;
    FILE *fp1,*fp2,*fp3;
    int i=0,j=0;
    char c;
    fp1=fopen("insert.txt","r");
    if(fp1==NULL)
    {
        printf("Reading failed!\n");
        exit(0);
    }
    else
    {
        printf("the insert file is:");
        do
        {
            fscanf(fp1,"%d",&m[i]);
            printf("%d ",m[i]);
            i++;

        }while(!feof(fp1));
        printf("\n");
        p=i;
    }
    i=0;
    fp2=fopen("delete.txt","r");
    if(fp2==NULL)
    {
        printf("Reading failed!\n");
        exit(0);
    }
    else
    {
        printf("the delete file is:");
        do
        {
            fscanf(fp2,"%d",&n[i]);
            printf("%d ",n[i]);
            i++;

        }while(!feof(fp2));
        printf("\n");
        q=i;
    }
    i=0;
    fp3=fopen("search.txt","r");
    if(fp3==NULL)
    {
        printf("Reading failed!\n");
        exit(0);
    }
    else
    {
        printf("the search file is:");
        do
        {
            fscanf(fp3,"%d",&s[i]);
            printf("%d ",s[i]);
            i++;

        }while(!feof(fp3));
        printf("\n");
        t=i;
    }
    for(i=0;i<p;i++)
    {
        root=AVL_Insert(root,m[i]);
    }
    printf("the tree is: ");
    print(root);
    printf("\n\nthe tree's height is %d\n",root->height);
    for(i=0;i<q;i++)
    {
        root=AVL_Delete(root,n[i]);
    }
  printf("\nafter deleted: ");
    print(root);
    NODE y;
    printf("\n\nsearch result:\n");
    for(i=0;i<t;i++)
    {
        y=AVL_Find(root,s[i]);
        if (y==NULL)
        {
            printf("没有查找到%d\n",s[i]);
        }
        else
        {
            printf("\n%d  所在节点的高度：%d\n",s[i],y->height);
            if (NULL!=y->left)
            {
                printf("   所在节点的左孩子：%d\n",y->left->data);
            }
            if (NULL!=y->right)
            {
                printf("   所在节点的右孩子：%d\n",y->right->data);
            }

        }
    }
}
