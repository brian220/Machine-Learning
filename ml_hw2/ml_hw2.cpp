#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>
#include<string.h>
#include<math.h>
using namespace std;

class ecoli{
public:
    int index;
    char eco_name[20];
    float feature[9];
    char eco_class[20];
    vector <int> nearest;
};

class node{
public:
    int traversed;
    int devide_feature;
    ecoli node_ecoli;
    vector <ecoli> data_set;
    int traveled;
    node * left;
    node * right;
    node * parent;
    node(){traveled = 0;}
};

vector <ecoli> read_file(char * file_str ,int n){

    vector <ecoli> ecoli_set;

    FILE* pfile;
    pfile = fopen(file_str,"r");

    if(!pfile){

        printf("Open file error!\n");
    }

    char str[50];
    fscanf(pfile,"%s\n",str);

    for(int i = 0;i < n; i++){

        ecoli temp_ecoli;
        fscanf(pfile,"%d,", &temp_ecoli.index);

        char c;
        while(fscanf(pfile,"%c",&c)){

            if(c == ','){

                break;
            }
        }

        fscanf(pfile,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,",
                      &temp_ecoli.feature[0],&temp_ecoli.feature[1],
                      &temp_ecoli.feature[2],&temp_ecoli.feature[3],
                      &temp_ecoli.feature[4],&temp_ecoli.feature[5],
                      &temp_ecoli.feature[6],&temp_ecoli.feature[7],
                      &temp_ecoli.feature[8]);

        fscanf(pfile,"%s",temp_ecoli.eco_class);
        ecoli_set.push_back(temp_ecoli);
    }

    return ecoli_set;
}



int sort_devide;
bool compare (ecoli a, ecoli b){

    return a.feature[sort_devide] < b.feature[sort_devide];
}

//gernerate left and right child
void generate_child(node * current){

    current -> left = new node;
    current -> left -> parent = current;
    current -> left -> devide_feature = (current -> devide_feature > 7)?0 : (current -> devide_feature + 1);

    if(current -> data_set.size() > 2){

        current -> right = new node;
        current -> right -> parent = current;
        current -> right -> devide_feature = (current -> devide_feature > 7)?0 : (current -> devide_feature + 1);
    }
    else{

        current -> right = NULL;
    }
}

//devide the dataset into left and right child
void devide_node(node * current){

    sort_devide = current -> devide_feature;
    sort(current -> data_set.begin(),current -> data_set.end(),compare);

    for(int i = 0; i < current -> data_set.size(); i++){

        if(i < current -> data_set.size() / 2){

            current -> left -> data_set.push_back(current -> data_set[i]);
        }
        else if(i == current -> data_set.size() / 2){

            current -> node_ecoli = current -> data_set[i];
        }
        else{

            current -> right -> data_set.push_back(current -> data_set[i]);
        }
    }
}

void build_tree(node * current){

    if(current -> data_set.size() == 0){

        current = NULL;
        return;
    }
    else if(current -> data_set.size() == 1){

        current -> node_ecoli = current -> data_set[0];
        current -> left = NULL;
        current -> right = NULL;
        return;
    }

    generate_child(current);

    devide_node(current);

    build_tree(current -> left);
    if(current -> data_set.size() > 2){

        build_tree(current -> right);
    }
    return;
}

float compute_dis(node * current,ecoli query){

    float re_dis = 0.0;
    for(int i = 0; i < 9 ; i ++){

        re_dis += (current -> node_ecoli.feature[i] - query.feature[i])*(current -> node_ecoli.feature[i] - query.feature[i]);
    }
    re_dis = sqrt(re_dis);

    return re_dis;
}

float compute_bind_dis(node * current,ecoli query){

    float re_bind_dis;
    re_bind_dis = fabs((query.feature[current -> devide_feature]) - (current -> node_ecoli.feature[current -> devide_feature]));

    return re_bind_dis;

}

//go down to the leaf
node * descend_tree(node * root, ecoli query){

    node * current = root;
    node * leaf;

    while(current != NULL){

        if(query.feature[current -> devide_feature] <= current -> node_ecoli.feature[current -> devide_feature]){

            if(current -> left == NULL && current -> right != NULL){

                current = current -> right;
            }
            else if(current -> left == NULL){

                leaf = current;
                current = current -> left;
            }
            else{

                current = current -> left;
            }
        }
        else{

           if(current -> right == NULL && current -> left != NULL){

                current = current -> left;
            }
            else if(current -> right == NULL){

                leaf = current;
                current = current -> right;
            }
            else{

                current = current -> right;
            }
        }
      }

    return leaf;
}

class get_neighbor{
public:
    float dis;
    int index;
} ;

vector <int> traverse_tree(node * root, ecoli query, int k){

    node * renode = descend_tree(root, query);
    node * past_node = renode;

    vector <int> neighbors;
    vector <get_neighbor> k_nearest;
    neighbors.resize(k + 1);
    k_nearest.resize(k + 1);

    for(int i = 0; i <= k ;i ++){

        k_nearest[i].dis = 999999.9;
        k_nearest[i].index = - 1;
    }

    float best_dis = 999999.9;

    while(renode != NULL){

            if(renode -> traveled == 1){

               past_node = renode;
               renode = renode -> parent;
               continue;
            }
            //compute the min_dist
            if(compute_dis(renode, query) < k_nearest[k - 1].dis){

                renode -> traveled = 1;
                int i;
                for(i = k - 1; i >= 0; i --){

                    if(compute_dis(renode, query) < k_nearest[i].dis){

                        k_nearest[i + 1] = k_nearest[i];
                    }
                    else{

                        break;
                    }
                }
                i ++;
               k_nearest[i].dis = compute_dis(renode, query);
               k_nearest[i].index = renode -> node_ecoli.index;
           }

           //child
           if(renode -> left == NULL && renode -> right == NULL){

               past_node = renode;
               renode = renode -> parent;
           }//parent
           else if(past_node == renode -> left){

               renode -> left = NULL;
               if(compute_bind_dis(renode, query) < k_nearest[k - 1].dis && renode -> right != NULL){

                   renode = descend_tree(renode -> right, query);
               }
               else{

                   past_node = renode;
                   renode = renode -> parent;
               }
           }
           else{

               renode -> right = NULL;
               if(compute_bind_dis(renode, query) < k_nearest[k - 1].dis && renode -> left != NULL){

                   renode = descend_tree(renode -> left, query);
               }
               else{

                   past_node = renode;
                   renode = renode -> parent;
               }
           }
       }

    for(int i = 0; i < k; i ++){

        neighbors[i] = k_nearest[i].index;
    }

    return neighbors;
}
bool check_type_correct(int i,int max_type_index,vector <ecoli> test_ecoli_set){

    if(max_type_index == 0){

        if(!strcmp(test_ecoli_set[i].eco_class,"cp")){

                return true;
        }
    }
    else if(max_type_index == 1){


        if(!strcmp(test_ecoli_set[i].eco_class,"im")){

                return true;
        }
    }
    else if(max_type_index == 2){


        if(!strcmp(test_ecoli_set[i].eco_class,"pp")){

                return true;
        }
    }
    else if(max_type_index == 3){


        if(!strcmp(test_ecoli_set[i].eco_class,"imU")){

                return true;
        }
    }
    else if(max_type_index == 4){


        if(!strcmp(test_ecoli_set[i].eco_class,"om")){

                return true;
        }
    }
    else if(max_type_index == 5){


        if(!strcmp(test_ecoli_set[i].eco_class,"omL")){

                return true;
        }
    }
    else if(max_type_index == 6){


        if(!strcmp(test_ecoli_set[i].eco_class,"inL")){

                return true;
        }
    }
    else if(max_type_index == 7){


        if(!strcmp(test_ecoli_set[i].eco_class,"imS")){

                return true;
        }
    }
    return false;
}

float knn_vote(vector < vector<int> > neighbors_set, vector <ecoli> train_ecoli_set, vector <ecoli> test_ecoli_set, int k){

    int correct = 0;
    for(int i = 0; i < neighbors_set.size(); i ++){

        int type[8] = {0};
        for(int j = 0; j < neighbors_set[i].size(); j ++){

            if(!strcmp(train_ecoli_set[neighbors_set[i][j]].eco_class,"cp")){

                type[0] ++;
            }
            else if(!strcmp(train_ecoli_set[neighbors_set[i][j]].eco_class,"im")){

                type[1] ++;
            }
            else if(!strcmp(train_ecoli_set[neighbors_set[i][j]].eco_class,"pp")){

                type[2] ++;
            }
            else if(!strcmp(train_ecoli_set[neighbors_set[i][j]].eco_class,"imU")){

                type[3] ++;
            }
            else if(!strcmp(train_ecoli_set[neighbors_set[i][j]].eco_class,"om")){

                type[4] ++;
            }
            else if(!strcmp(train_ecoli_set[neighbors_set[i][j]].eco_class,"omL")){

                type[5] ++;
            }
            else if(!strcmp(train_ecoli_set[neighbors_set[i][j]].eco_class,"inL")){

                type[6] ++;
            }
            else if(!strcmp(train_ecoli_set[neighbors_set[i][j]].eco_class,"imS")){

                type[7] ++;
            }
        }
        int max_type = -999;
        int max_type_index = 0;
        for(int j = 0; j < 8; j ++){

            if(max_type < type[j]){

                max_type = type[j];
                max_type_index = j;
            }
        }
        if(check_type_correct(i, max_type_index, test_ecoli_set)){

            correct ++;
        }
    }

    float knn_accuracy = (float)((float)correct / 36.0);
    return knn_accuracy;
}

//test the tree and show the accuracy
void test_kd_tree(FILE * output, int k){


    vector <ecoli> train_ecoli_set = read_file("train.csv",300);
    vector <ecoli> test_ecoli_set = read_file("test.csv",36);
    vector < vector<int> > neighbors_set;

    for(int i = 0; i < 36; i++){

         node * root;
         root = new node;
         root -> parent = NULL;
         root -> data_set = train_ecoli_set;
         root -> devide_feature = 0;
         build_tree(root);

         vector <int> temp_neighbors;
         temp_neighbors = traverse_tree(root, test_ecoli_set[i], k);

         vector <int> neighbors;
         for(int j = 0; j < k ;j ++){

             neighbors.push_back(temp_neighbors[j]);
         }
         neighbors_set.push_back(neighbors);
    }

    float knn_accuracy;
    knn_accuracy = knn_vote(neighbors_set, train_ecoli_set, test_ecoli_set, k);

    fprintf(output, "KNN accuracy: %f\n", knn_accuracy);
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < k; j++){

            fprintf(output, "%d ", neighbors_set[i][j]);
        }
        fprintf(output, "\n");
    }
    fprintf(output, "\n");
}

int main(){

    FILE * output;
    output=fopen("a.txt", "wt");

    test_kd_tree(output, 1);
    test_kd_tree(output, 5);
    test_kd_tree(output, 10);
    test_kd_tree(output, 100);
    return 0;
}
