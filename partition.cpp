#include <cstdio>
#include <cstring>
#include <string>
#include <malloc.h>
#include <algorithm>
#include "partition.h"
#include "trie.h"
//#define DEBUG

static int chartoint(char * line, int s, int t) {
	int ans = 0;
	for (int i = s; i <= t; i++) {
		ans *= 10;
		ans += line[i] - '0';
	}
	return ans;
}

static void getmn(char* line, int& m, int& n) {
	int count = 0,start=0,end=0;
	for (int i = 0; line[i] != '\0'; i++) if(line[i]==' '||line[i]=='\t'||line[i]=='\n'){
		end = i - 1;
		switch (count) {
		case 2: {
			n = chartoint(line, start, end);
			break;
		}
		case 3: {
			m = chartoint(line, start, end);
			break;
		}
		default: {
			break;
		}
		}
		start = i + 1;
		count++;
	}
}

static char* filename(int intname, int postfix) {//返回指向"intname_postfix"的指针
	int digit_num = 0,tem=1;
	do {
		tem *= 10;
		digit_num++;
	} while (intname / tem);
	char * ans1 = (char*)malloc(digit_num + 1);//+1是\0的位置
	ans1[digit_num] = '\0';
	do{
		ans1[--digit_num] = '0' + intname % 10;
		intname /= 10;
	} while (intname);
	digit_num = 0; tem = 1;
	do {
		tem *= 10;
		digit_num++;
	} while (postfix / tem);
	char* ans2 = (char*)malloc(digit_num + 1);
	ans2[digit_num] = '\0';
	do{
		ans2[--digit_num] = '0' + postfix % 10;
		postfix /= 10;
	} while (postfix);
	int ans1_len = strlen(ans1), ans2_len = strlen(ans2);
	char * ans = (char*)malloc(ans1_len + ans2_len + 1 + 1 +4);//\0,_,和.txt(.cnf)的空间
	memcpy(ans, ans1, ans1_len);
	ans[ans1_len] = '_';
	memcpy(ans + ans1_len+1, ans2, ans2_len);
	ans[ans1_len + ans2_len + 1] = '.';
	ans[ans1_len + ans2_len + 2] = 't';
	ans[ans1_len + ans2_len + 3] = 'x';
	ans[ans1_len + ans2_len + 4] = 't';
	ans[ans1_len + ans2_len + 5] = '\0';
	free(ans1); free(ans2);
	return ans;
}

static bool cmp(const std::vector<int>& a, const std::vector<int>& b) {//字典序
	if (a.size() < b.size()) return true;
	else if (a.size() > b.size()) return false;
	else {
		for (int i = 0; i < a.size(); i++) {
			if (a[i] < b[i]) return true;
			if (a[i] > b[i]) return false;
		}
		return false;
	}
}

int partition(const char* infile, int nparts, double range) {   //传入待分文件名，返回分出了几个文件，失败返回0
 
	int m, n;                                                   //m是总文件的句子数，n是总文件的变量数
	FILE* in = fopen(infile, "r");
	if (!in) throw infile;

	char buf[200];
	do {
		fgets(buf, 200, in);
	} while (buf[0] == 'c');

	getmn(buf, m, n);                                            //通过p开头的行解读出m和n
	int size_per_file = m / nparts;
	if (m % nparts) size_per_file++;                             //当nparts < size_per_file时没有问题。

	std::vector<std::vector<int> > file_contant(size_per_file); //用于存放每个分文件的内容然后排序。

	int filename_now = 1;                                        //当前文件名

	for (int p = 0; p < m; p += size_per_file) {                

		char* deleter = filename(filename_now, 0);               //deleter用于释放内存, 这里创建了文件1_0, 2_0, ..., k_0 (k个文件)
		FILE* now = fopen(deleter, "w");

		int cur_file_size = (m > filename_now * size_per_file) ? //剩余的够填满当前文件，cur_file_size = size_per_file, 不够则 cur_file_size = 剩下的
			size_per_file : m - (filename_now - 1) * size_per_file;

		fprintf(now, "p cnf %d %d\n", n, cur_file_size);
		file_contant.clear();

		std::vector<int> clause;
		for (int i = 0; i < cur_file_size; i++) {
			int tem;
			clause.clear();
			while (fscanf(in, "%d", &tem) && tem) clause.push_back(tem);
			sort(clause.begin(), clause.end());
			
			//remove duplicate elements in a clause
			int del = 0;
			for (int i = 0, j = 0; i < clause.size(); i++, j++) {
				clause[j] = clause[i];
				while (i < (int)clause.size() - 1 && 
					clause[i] == clause[i + 1])
				{ i++, del++; }
			}
			clause.erase(clause.begin() + clause.size() - del, clause.end());

			file_contant.push_back(clause);
		}
		sort(file_contant.begin(), file_contant.end(), cmp);

		for (int i = 0; i < file_contant.size(); i++) {
			for (int j = 0; j < file_contant[i].size(); j++)
				fprintf(now, "%d ", file_contant[i][j]);
			fprintf(now, "0\n");
		}

		fclose(now);
		free((void *)deleter);
		filename_now++;
	}

	fclose(in);
	for (int i = 1; i < filename_now; i++) {
		Trie tree;
		char * deleter1 = filename(i, i - 1);

		FILE * now = fopen(deleter1, "r");
		fgets(buf, 200, now);
		int m_i, n_i;                                                                      //第i个文件的句子数和变量数
		getmn(buf, m_i, n_i);

		std::vector<int> clause;
		int tem;

		char * deleter2 = filename(i, i);
		FILE * out = fopen(deleter2, "w");
		fprintf(out, "                                                                 \n");//文件首加一个长空行，给写p cnf n m留地方

		int m_i_cpy = m_i;
		for (int j = 0; j < m_i; j++) {                                                     //这个循环的作用是分文件自己筛自己，顺便建树
			clause.clear();
			while (fscanf(now, "%d", &tem) && tem) clause.push_back(tem);
			if (tree.if_implied(clause)) {
				m_i_cpy--;
#ifdef DEBUG
				for (int p = 0; p < clause.size(); p++) printf("%d ", clause[p]);
				printf("\n");
#endif
				continue;
			}
			tree.insert(clause);
			for (int k = 0; k < clause.size(); k++) fprintf(out,"%d ", clause[k]);
			fprintf(out, "0\n");
		}
		fseek(out, 0, SEEK_SET);
		fprintf(out, "p cnf %d %d", n_i, m_i_cpy);                                            //没加\n的原因是当时输出的长空行已经含有\n，这一行的内容是覆盖空行的空格。
		fclose(out);
		free(deleter2);
		fclose(now);
		free(deleter1);
		for (int j = 1; j < filename_now; j++) if(j != i) {                                   //用第i个文件做成筛子，筛第1,2,...,i-1,i+1,i+2,...,m个文件
			deleter2 = filename(j, i);
			out = fopen(deleter2, "w");
			deleter1 = filename(j, i - 1);
			now = fopen(deleter1, "r");
			fgets(buf, 200, now);
			getmn(buf, m_i, n_i);
			fprintf(out, "                                                                 \n");
			m_i_cpy = m_i;
			for (int k = 0; k < m_i; k++) {
				clause.clear();
				while (fscanf(now, "%d", &tem) && tem) clause.push_back(tem);
				if (tree.if_implied(clause)) {
					m_i_cpy--;
#ifdef DEBUG
					for (int p = 0; p < clause.size(); p++) printf("%d ", clause[p]);
					printf("\n");
#endif
					continue;
				}
				for (int p = 0; p < clause.size(); p++) fprintf(out, "%d ", clause[p]);
				fprintf(out, "0\n");
			}
			fseek(out, 0, SEEK_SET);
			fprintf(out, "p cnf %d %d", n_i, m_i_cpy);
			fclose(out);
			free(deleter2);
			fclose(now);
			free(deleter1);
		}
	}
#ifndef DEBUG
	for (int i = 1; i < filename_now; i++)
		for (int j = 0; j < filename_now - 1; j++) {
			char* d = filename(i, j);
			remove(d);
			free(d);
		}
#endif
	return filename_now - 1;
}

