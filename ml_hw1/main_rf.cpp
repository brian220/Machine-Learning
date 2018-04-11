#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <math.h>
#include <string.h>
#include <utility>
#include <algorithm>
#include <time.h>
#define tree_num 5
using namespace std;

class iris{
public:
    float s_length;
    float s_width;
    float p_length;
    float p_width;
    float feature_set[4];
    char  name[50];
};

class node{
public:
   vector <iris> dataset;
   float div_num;
   int div_feature;
   node* left;
   node* right;
   node* parent;
};


float entro(vector <iris> dataset){

    int iris_kind[3] = {};
    for(int i = 0;i < dataset.size();i ++)
    {
      if(!strcmp(dataset[i].name,"Iris-setosa")){

        iris_kind[0] ++;
      }
      else if(!strcmp(dataset[i].name,"Iris-versicolor")){

        iris_kind[1] ++;
      }
      else{

        iris_kind[2] ++;
      }
    }

    float return_en;
    float data_num = dataset.size();
    float kind0 = (iris_kind[0] == 0)? 0.0 : (-(iris_kind[0] / data_num) * log2((iris_kind[0] / data_num)));
    float kind1 = (iris_kind[1] == 0)? 0.0 : (-(iris_kind[1] / data_num) * log2((iris_kind[1] / data_num)));
    float kind2 = (iris_kind[2] == 0)? 0.0 : (-(iris_kind[2] / data_num) * log2((iris_kind[2] / data_num)));

    return_en = kind0 + kind1 + kind2;

    return return_en;
}

float rem(vector <iris> dataset1,vector <iris> dataset2){

   float entro1 = entro(dataset1);
   float entro2 = entro(dataset2);

   float data_size = dataset1.size() + dataset2.size();
   float return_rem;
   return_rem = entro1 * (dataset1.size()/data_size) + entro2 * (dataset2.size()/data_size);
   return return_rem;
}

float infor_g(vector <iris> dataset, float div_num, int feature){

   vector <iris> dataset1;
   vector <iris> dataset2;

   for(int i = 0;i < dataset.size(); i ++){
     if(dataset[i].feature_set[feature] >= div_num){

         dataset1.push_back(dataset[i]);
     }
     else{

         dataset2.push_back(dataset[i]);
     }
   }
   float ig;
   ig = entro(dataset) - rem(dataset1,dataset2);
   return ig;
}

int sort_feature = 0;
bool compare(iris a,iris b){
    if(sort_feature == 0){

      return (a.s_length < b.s_length);
    }
    else if(sort_feature == 1){

      return (a.s_width < b.s_width);
    }
    else if(sort_feature == 2){

      return (a.p_length < b.p_length);
    }
    else{

       return (a.p_width < b.p_width);
    }
}

pair <int,float> best_infor_g(vector <iris> dataset){

   pair <int,float> div_way;
   int best_feature;
   float best_div_num;
   float best_ig = -999.9;

   for(int i = 0; i < 4;i ++){
     sort_feature = i;
     sort(dataset.begin(),dataset.end(),compare);
     for(int j = 0;j < dataset.size() - 1;j ++){

       if(strcmp(dataset[j].name,dataset[j + 1].name)){

         float div_num = ((dataset[j].feature_set[i] + dataset[j + 1].feature_set[i])/ 2);
         float temp_ig = infor_g(dataset, div_num, i);

         if(temp_ig > best_ig){

            best_ig = temp_ig;
            best_div_num = div_num;
            best_feature = i;
         }
       }
     }
   }
   //cout << best_ig << endl;
   div_way.first = best_feature;
   div_way.second = best_div_num;
   return div_way;
}


void create_tree(node * current){

  if(entro(current -> dataset) == 0){

    return;
  }
  pair <int,float> div_way = best_infor_g(current -> dataset);

  current -> div_feature = div_way.first;
  current -> div_num = div_way.second;

  sort_feature = current -> div_feature;
  sort(current -> dataset.begin(),current -> dataset.end(),compare);

  current -> right = new node;
  current -> left  = new node;

  for(int i = 0;i < current -> dataset.size();i ++){

    if(current -> dataset[i].feature_set[current -> div_feature] >= current -> div_num){

        current -> right -> dataset.push_back(current -> dataset[i]);
    }
    else{
        current -> left -> dataset.push_back(current -> dataset[i]);
    }
  }

  create_tree(current -> right);
  create_tree(current -> left);
  return;
}

//return the flower kind "1 -> Iris-setosa""2 -> Iris-versicolor""3 -> Iris-verginica"
int test_tree(node * current,iris test_node){

  if(entro(current -> dataset) == 0){

    if(!strcmp(current -> dataset[0].name,"Iris-setosa")){

                return 1;
    }
    else if(!strcmp(current -> dataset[0].name,"Iris-versicolor")){

                return 2;
    }
    else{

                return 3;
    }
  }

  if(test_node.feature_set[current -> div_feature] >= current -> div_num){

    return test_tree(current -> right,test_node);
  }
  else{

    return test_tree(current -> left,test_node);
  }
}

int main()
{
    vector <iris> iris_set;
    FILE* pfile;
    pfile = fopen("iris.data","r");
    if(!pfile)
    {
        printf("xdd");
    }

    for(int i = 0;i < 150;i ++){

        iris temp_iris;
        fscanf (pfile,"%f,%f,%f,%f,", &temp_iris.s_length,&temp_iris.s_width,&temp_iris.p_length,&temp_iris.p_width);
        fscanf (pfile,"%s",temp_iris.name);
        iris_set.push_back(temp_iris);
        iris_set[i].feature_set[0] = iris_set[i].s_length;
        iris_set[i].feature_set[1] = iris_set[i].s_width;
        iris_set[i].feature_set[2] = iris_set[i].p_length;
        iris_set[i].feature_set[3] = iris_set[i].p_width;
    }

    vector <iris> r_create_set;
    srand(time(NULL));
    random_shuffle(iris_set.begin(), iris_set.end());

    float total_precision[3] = {};
    float total_recall[3] = {};
    float total_accuracy = 0.0;

    for(int i = 0;i < 5; i++){

      vector <iris> r_create_set;
      vector <iris> test_set;
      for(int j = 0;j < 150;j ++){

        if(j % 5 == i){

            test_set.push_back(iris_set[j]);
        }
        else{

            r_create_set.push_back(iris_set[j]);
        }
      }
      //random forest
      int vote[30][3];
      for(int j = 0; j < 30;j ++){

        for(int k = 0;k < 3;k ++){

           vote[j][k] = 0;
        }
      }

      for(int j = 0;j < tree_num;j ++){

        random_shuffle(r_create_set.begin(), r_create_set.end());
        node *root;
        root = new node;
        for(int k = 0;k < 60;k ++){

          root ->  dataset.push_back(r_create_set[k]);
        }
        create_tree(root);
        for(int k = 0;k < 30;k ++){

            if(test_tree(root,test_set[k]) == 1){

                vote[k][0] ++;
            }
            else if(test_tree(root,test_set[k]) == 2){

                vote[k][1] ++;
            }
            else{

                vote[k][2] ++;
            }
        }
      }

      //get the result of the voting
      int vote_result[30];

      //the number of flower(is,iv,ivg) is determine by the tree(/precision)
      int decide_feature[3] = {};

      int max_index;
      for(int j = 0;j < 30;j ++){

        int max_num = -999;
        for(int k = 0;k < 3;k ++){

            if(vote[j][k] > max_num){

                max_num = vote[j][k];
                max_index = k;
            }
        }

        vote_result[j] = max_index;
        decide_feature[max_index] ++;
      }
      //the decide of the tree is same as the true circumstances (precision/)(recall/)
      int right_decide[3] = {};

      //the number of each flower in the test set(/recall)
      int true_kind[3] = {};

     //the number of the tree get the right kind(accuracy/)
      int get_kind = 0;

      for(int j = 0;j < 30; j++){

        if(!strcmp(test_set[j].name,"Iris-setosa")){

            true_kind[0] ++;
            if(vote_result[j] == 0){

               get_kind ++;
               right_decide[0]++;
            }
        }
        else if(!strcmp(test_set[j].name,"Iris-versicolor")){

            true_kind[1] ++;
            if(vote_result[j] == 1){

               get_kind ++;
               right_decide[1]++;
            }
        }
        else{

            true_kind[2] ++;
            if(vote_result[j] == 2){

               get_kind ++;
               right_decide[2]++;
            }
        }
      }

    // cout << get_kind << endl;
     total_precision[0] += ((float)right_decide[0])/((float)decide_feature[0]);
     total_precision[1] += ((float)right_decide[1])/((float)decide_feature[1]);
     total_precision[2] += ((float)right_decide[2])/((float)decide_feature[2]);

     total_recall[0] += ((float)right_decide[0])/((float)true_kind[0]);
     total_recall[1] += ((float)right_decide[1])/((float)true_kind[1]);
     total_recall[2] += ((float)right_decide[2])/((float)true_kind[2]);

     total_accuracy += ((float)get_kind)/30.0;
    }
    cout << total_accuracy/5.0 << endl;
    for(int i = 0; i < 3; i++){

        cout << total_precision[i] / 5.0 << " " << total_recall[i] / 5.0 << endl;
    }
}

