#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fields.h"
#include "dllist.h"
int main(int argc, char **argv)
{
	FILE *dosya = fopen("txt/output.txt", "a+");
	char *tr = "tr", *eng = "eng";
	int wordCount = 0, translateCount = 0;
	char *lang = argv[1];
	IS is = new_inputstruct(argv[2]), ss = new_inputstruct(argv[3]);
	Dllist OutWords, tmpOutWords;

	if (argc != 4)
	{
		fprintf(stderr, "usage:test language inputname filename\n");
		exit(1);
	}

	OutWords = new_dllist();

	if (lang == NULL)
	{
		perror(argv[1]);
		exit(1);
	}
	if (is == NULL)
	{
		perror(argv[2]);
		exit(2);
	}

	if (ss == NULL)
	{
		perror(argv[3]);
		exit(3);
	}
	char st = 't';

	while (get_line(is) >= 0)
	{
		for (int i = 0; i < is->NF; i++)
		{
			while (get_line(ss) >= 0)
			{
				for (int j = 0; j < ss->NF; j++)
				{
					if (strcmp(is->fields[i], ss->fields[j]) == 0)
					{
						if (strcmp(lang, tr) == 0)
						{
							if ((j + 1) != 3)
							{
								dll_append(OutWords, new_jval_s(strdup(ss->fields[j + 1])));
								translateCount++;
								st = 'f';
							}
						}
						if (strcmp(lang, eng) == 0)
						{
							if ((j - 1) != 0)
							{
								dll_append(OutWords, new_jval_s(strdup(ss->fields[j - 1])));
								translateCount++;
								st = 'f';
							}
						}
					}
				}
			}
			wordCount = is->NF;
			ss = new_inputstruct(argv[3]);
			if (st == 't')
				dll_append(OutWords, new_jval_s(strdup(is->fields[i])));
			st = 't';
		}
	}

	tmpOutWords = OutWords->flink;
	while (tmpOutWords != OutWords)
	{
		printf("%s ", tmpOutWords->val.s);
		fprintf(dosya, "%s ", tmpOutWords->val.s);
		tmpOutWords = tmpOutWords->flink;
	}

	printf("\n\n-ÖZET BİLGİ-\nKarşılaştırılan Kelime Sayısı:%d\nÇevirilen Kelime Sayısı:%d\nÇevirelemeyen Kelime Sayısı: %d\n", wordCount, translateCount, (wordCount - translateCount));

	jettison_inputstruct(is);
	jettison_inputstruct(ss);

	return 0;
}
