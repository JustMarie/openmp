#include <iostream.h>
#include <math.h>
#include <string>
using namespace std; 
  
int serial_search(string* pattern, string* txt) {
    int pattern_len = patter.length(); 
    int text_len = txt.length(); 
    int pos = -1;

    for (int i = 0; i <= text_len - pattern_len; i++) { 
        for (int j = 0; j < pattern_len; j++) 
            if (txt[i + j] != pattern[j]) 
                return -1; 
        if (j == pattern_len) 
            pos = i; 
    } 
    return pos;
} 

int parallel_search(int p, string* patter, string* txt) {
    int pattern_len = patter.length(); 
    int text_len = txt.length(); 
 
    omp_set_num_threads(p);
    int i;

    int substr_len = int(cell((txt_len + (p-1)*pattern_len)/p)); 
    // set the substring length each thread will go through
    int pos = -1  
  
#pragma parallel for private(i) shared(pattern, text)
    for (i=0; i<p; i++) {
        string thread_substr = txt.substr(i*substr_len, substr_len);
        int thr_pos = serial_search(pattern, thread_substr);
#pragma omp critical
        { if (thr_pos != -1) 
            pos = thr_pos;
        }
    }
    return pos;
}

 
int main() 
{ 
    string txt = "AABAACAADAABAAABAA"; 
    string ptrn = "AABA";
    int serial_pos, par_pos;
    int num_thread = 8;
    
    double ser_start = omp_get_wtime(); 
    serial_pos = serial_search(ptrn, txt); 
    double ser_stop = omp_get_wtime(); 

    double par_start = omp_get_wtime(); 
    par_pos = parallel_search(p, ptrn, txt); 
    double par_stop = omp_get_wtime(); 

    if (serial_pos==-1) 
        cout << "Pattern isn`t found by serial search" << endl;
    else 
        cout << "Pattern found at index " << serial_pos << endl;
    cout << " Wasted time is " << ser_stop-ser_start << "" sec" << endl;
    cout << endl;
    if (par_pos==-1) 
        cout << "Pattern isn`t found by parallel search" << endl;
    else 
        cout << "Pattern found at index " << parallel_pos << endl;
    cout << " Wasted time with " << num_thread << " threads is " << par_stop-par_start << "" sec" << endl;

    return 0; 
} 

