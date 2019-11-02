#include<stdio.h>
#include<malloc.h>
#define MAX 100
int m[MAX];
int n[MAX];
int s[MAX];
int p=0;
int q=0;
int t=0;
typedef struct Node//ƽ��������Ľṹ��
{
    int data;
    struct Node *left;
    struct Node *right;
    int height;  //�Դ˽ڵ�Ϊ�������ĸ߶ȣ�
    unsigned int freq;//�˽ڵ㱣������ݳ��ֵ�Ƶ��
}Node,*NODE;

int NodeHeight(NODE  ptrTree)//�����ĸ߶�-1
{
    return ptrTree==NULL ? -1 :ptrTree->height;
}
int max(int a,int b)
{
    return a<b ? b : a;
}

NODE LL(NODE k1)//LL��ת
{
    NODE k= k1->left;  //����ڵ��������
    k1->left = k->right;   //k�ڵ����������Ϊk1��������
    k->right =k1;          //��k1��k��������
    k1->height= max(NodeHeight(k1->left),NodeHeight(k1->right))+1;
    k->height= max(NodeHeight(k->left),NodeHeight(k->right))+1;
    return k;
}

NODE RR(NODE k2)
{
    NODE k= k2->right;  //����ڵ��������
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

NODE LeftBalance(NODE node)//��ƽ�⴦��
{
    NODE ptrTmp=node->left;
    if(NodeHeight(ptrTmp->left)-NodeHeight(ptrTmp->right)==-1)//�������������������������������
        node=LR(node); //LR
    else
        node=LL(node); //LL
    return node;
}

NODE RightBalance(NODE node)//��ƽ�⴦��
{
    NODE ptrTmp=node->right;
    if(NodeHeight(ptrTmp->right)-NodeHeight(ptrTmp->left)==-1) //���������������ߣ�˵���������������
        node=RL(node); //RL
    else
        node=RR(node);  //RR
}

NODE AVL_Insert(NODE node,int x)
{

    if(NULL==node) //�ҵ�����Ľڵ��λ��
    {
        node =(NODE)malloc(sizeof(Node));
        node->data=x;
        node->height=0;
        node->freq = 1;
        node->left=NULL;
        node->right=NULL;
       // printf("~%d",node->data);
    }
    else if(x<node->data)  //������������
    {
       // printf("lll");
        node->left=AVL_Insert(node->left,x);
        if (NodeHeight(node->left)-NodeHeight(node->right)==2)//�ж��Ƿ��ƻ�ƽ��
            node=LeftBalance(node);  //��ƽ�⴦��
    }
    else if(node->data<x)   //������������
    {
        node->right=AVL_Insert(node->right,x);
        if (NodeHeight(node->right)-NodeHeight(node->left)==2)//�ж��Ƿ��ƻ�ƽ��
            node=RightBalance(node); //��ƽ�⴦��

    }
    else
        node->freq++;

    node->height = max(NodeHeight(node->left),NodeHeight(node->right)) +1 ;  //�������ĸ߶�
    return node;
}

NODE AVL_Find(NODE node,int x)
{
    if (node==NULL)  //û�ҵ�Ԫ��
    {
        return NULL;
    }
    else if(x<node->data)
    {
         AVL_Find(node->left,x); //���������������
    }
    else if(node->data<x)
    {
         AVL_Find(node->right,x); //���������������
    }
    else //���
        return node;
}

NODE AVL_Delete(NODE node,int x)
{
    if(NULL==node)  //����ֱ�ӷ���
        return;
    if(x<node->data)  //���������в���
    {
         node->left=AVL_Delete(node->left,x);
         if(NodeHeight(node->right)-NodeHeight(node->left)==2)  //����ƽ�⴦��
          node=RightBalance(node);
    }
    else if(node->data<x)  //���������в���
    {
        node->right=AVL_Delete(node->right,x);
         if(NodeHeight(node->left)-NodeHeight(node->right)==2)  //����ƽ�⴦��
          node=LeftBalance(node);
    }
    else //�ҵ�Ҫɾ����Ԫ�ؽڵ�
    {
        if(node->left==NULL) //������Ϊ��
        {
            NODE ptrTmp = node;
            node=node->right;         //���Һ��Ӵ���˽ڵ�
            free(ptrTmp);            //�ͷ��ڴ�
        }
        else if(node->right==NULL)  //������Ϊ��
        {
            NODE ptrTmp = node;
            node=node->left;       //�����Ӵ���˽ڵ�
            free(ptrTmp);
        }
        else   //������������Ϊ��
        {
            //������������С���ݻ�����������С���ݴ���ýڵ�
            NODE ptrTmp=node->left;  //���������в���
            while(ptrTmp->right!=NULL)
                ptrTmp=ptrTmp->right;//ptrTmpָ���������е����Ԫ��
            node->data =  ptrTmp->data;
            node->left=AVL_Delete(node->left,ptrTmp->data);  //�ݹ��ɾ���ýڵ�
        }
    }

    //���½ڵ�ĸ߶�
    if(node)
        node->height = max(NodeHeight(node->left),NodeHeight(node->right));

    return node;
}

//�������
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
            printf("û�в��ҵ�%d\n",s[i]);
        }
        else
        {
            printf("\n%d  ���ڽڵ�ĸ߶ȣ�%d\n",s[i],y->height);
            if (NULL!=y->left)
            {
                printf("   ���ڽڵ�����ӣ�%d\n",y->left->data);
            }
            if (NULL!=y->right)
            {
                printf("   ���ڽڵ���Һ��ӣ�%d\n",y->right->data);
            }

        }
    }
}
