#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <math.h>
#include <string.h>
#include <utility>
#include <algorithm>
#include <time.h>
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

bool test_tree(node * current,iris test_node, int * feature_decide){

  if(entro(current -> dataset) == 0){

    if(!strcmp(current -> dataset[0].name,"Iris-setosa")){

                feature_decide[0] ++;
    }
    else if(!strcmp(current -> dataset[0].name,"Iris-versicolor")){

                feature_decide[1] ++;
    }
    else{

                feature_decide[2] ++;
    }

    if(!strcmp(test_node.name,current -> dataset[0].name)){

      return true;
    }
    else{

      return false;
    }
  }

  if(test_node.feature_set[current -> div_feature] >= current -> div_num){

    return test_tree(current -> right,test_node,feature_decide);
  }
  else{

    return test_tree(current -> left,test_node,feature_decide);
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
    srand(time(NULL));
    random_shuffle(iris_set.begin(), iris_set.end());

    float precision_sum[3] = {0};
    float recall_sum[3] = {0};
    float accuracy_sum = 0.0;

    for(int i = 0;i < 5;i ++)
    {
        node* root;
        root = new node;
        vector <iris> test_set;

        int true_num = 0;
        int setosa_num = 0;
        int versicolor_num = 0;
        int virginica_num = 0;

        int setosa_true = 0;
        int versicolor_true = 0;
        int virginica_true = 0;
        int feature_decide[3] = {};

        for(int j = 0;j < 150;j ++){

          if(j%5 == i){

            test_set.push_back(iris_set[j]);
          }
          else{

            root -> dataset.push_back(iris_set[j]);
          }
        }
        create_tree(root);
        for(int k = 0;k < 30;k ++){

           if(!strcmp(test_set[k].name,"Iris-setosa")){

                setosa_num ++;
                if(test_tree(root,test_set[k],feature_decide) == true){
                  true_num ++;
                  setosa_true ++;
                }
            }
            else if(!strcmp(test_set[k].name,"Iris-versicolor")){

                 versicolor_num ++;
                 if(test_tree(root,test_set[k],feature_decide) == true){
                    true_num ++;
                    versicolor_true ++;
                 }
            }
            else{

                 virginica_num ++;
                 if(test_tree(root,test_set[k],feature_decide) == true){
                   true_num ++;
                   virginica_true ++;
                 }
            }


          }
        precision_sum[0] += ((float)setosa_true)/((float)feature_decide[0]);
        precision_sum[1] += ((float)versicolor_true)/((float)feature_decide[1]);
        precision_sum[2] += ((float)virginica_true)/((float)feature_decide[2]);

        recall_sum[0] += ((float)setosa_true)/ ((float)setosa_num);
        recall_sum[1] += ((float)versicolor_true)/ ((float)versicolor_num);
        recall_sum[2] += ((float)virginica_true)/ ((float)virginica_num);

        accuracy_sum += ((float)true_num)/ 30.0;
        }
      cout <<  accuracy_sum/5.0 << endl;
      for(int i = 0;i < 3; i++){

        cout << (precision_sum[i]/5.0) << " " << (recall_sum[i] / 5.0) << endl;
      }
      return 0;
    }



