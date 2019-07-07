#include<fstream>
#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<map>
#include<cctype>
#include<sstream>
#include<numeric>
#include<math.h>

using namespace std;

#define file_no 7

bool cmp(const pair<string,vector<float> >& x,const pair<string,vector<float> >& y)
{
   return *(x.second.begin()) > *(y.second.begin());
}

bool cmp_tfidf(const pair<string,float>& x,const pair<string,float>& y)
{
   return x.second > y.second;
}

vector<pair<string,vector<float> > > sort_words (map<string,vector<float> > ret)
{
    //sort the words based on the count
    
    vector<pair<string,vector<float> > > vec; 
    copy(ret.begin(),ret.end(),back_inserter(vec));
    sort(vec.begin(),vec.end(),cmp);

    //display the sorted list optional
    for(vector<pair<string,vector<float> > >::const_iterator it= vec.begin();it != vec.end();++it)
    {
        cout<<it->first<<" count: "<<*it->second.begin()<<endl;
    }
    return vec;
}

vector<pair<string,float> > sort_tfidf (const map<string,float>& t)
{
    //sort based on tfidf
    vector<pair<string,float> > vec; 
    copy(t.begin(),t.end(),back_inserter(vec));
    sort(vec.begin(),vec.end(),cmp_tfidf);
    
    //display
    for(vector<pair<string,float> >::const_iterator it=vec.begin();it!=vec.end();++it)
    {
        cout<<it->first<<" tfidf: "<<it->second<<endl;
    }
    return vec;
}


//calculates the term count for a given document
void term_count(istream& in,map<string,vector<float> >& ret )
{
    string x;
    while(in>>x)
    {
        if (ret[x].empty())
            ret[x].push_back(1);
        else
            *(ret[x].begin())=*(ret[x].begin())+1;
    }
    in.clear();
    in.seekg(0);
}



//increases df if word is in the given document
void DF_helper(ifstream& in,map<string,vector<float> >& ret)
{
    string x;
    vector<string> visited;
    while(in>>x)
    {
        if (find(visited.begin(),visited.end(),x)==visited.end()) // if x not in visited
        {
            visited.push_back(x);
            if(ret[x].empty()==1) // create a value for its term freq
                 ret[x].push_back(0);                      

            if(ret[x].size() == 1)
                ret[x].push_back(1);
            else
                ret[x][1]++;
        }
    }
}

void DF(ifstream myFiles[],map<string,vector<float> >& tfdf)
{
    for(int i=0;i<file_no;i++)
    { 
         DF_helper(myFiles[i],tfdf);
    }
}

int word_count(map<string,vector<float> > tfdf)
{
    int sum=0;
    for (map<string,vector<float> >::const_iterator it = tfdf.begin();it != tfdf.end();++it)
    {
         sum=sum+it->second[0];    
    }
    return sum;
}

map<string,float> calc_tf_idf(ifstream myFiles[])
{
    map<string,float> tf_idf;

    map<string,vector<float> > tfdf;    
    //term freq for the first document
    term_count(myFiles[0],tfdf);
    
    //count of all words in document
    int count = word_count(tfdf);

    //calculate df for all file
    DF(myFiles,tfdf);

    //calculate term freq and inverse document freq
    for (map<string,vector<float> >::iterator it = tfdf.begin();it != tfdf.end();++it)
    {
        it->second[0]=(it->second[0])/count;
        it->second[1]=log(file_no/it->second[1]);
    }

    // calculate tf_idf and storing it in a new map 
    for (map<string,vector<float> >::const_iterator it = tfdf.begin();it != tfdf.end();++it)
    {
        //removing word which don't have term freq
        if (it->second[0] != 0)
            tf_idf[it->first]= it->second[0] * it->second[1];
    }

    //sort optional or outside
    return tf_idf;
}


int main()
{
    //loads the file in the array
    ifstream myFiles[file_no];
    for(int i = 0; i < file_no; i++){
        ostringstream filename;
        filename << "file" << i <<".txt";
        myFiles[i].open(filename.str());
    }
    //term count
    
    //map<string,vector<float> > tc;
    //term_count(myFiles[0],tc);
    
    //sorted vector based on count  
    //vector<pair<string,vector<float> > > sorted_word_count;
    //sorted_word_count = sort_words(tc);  


    map<string,float> tf_idf =calc_tf_idf(myFiles);
    //sorted vector based on tfidf value  
    vector<pair<string,float> > sorted_tfidf = sort_tfidf(tf_idf);

    return 0;    
}