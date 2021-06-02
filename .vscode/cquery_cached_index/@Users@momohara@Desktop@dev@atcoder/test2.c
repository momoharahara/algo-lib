//全体の文字列の初めと終わりに空白を入れて、空白も込みであっているかの判定をする。(単語として判定するため)
#include <stdio.h>
#include <string.h>

int getstring(char *, int);
int check(char *, char *, int, int);

int getstring(char *s, int size) {
    int c, i = 0;

    while(i < size && (c = getchar()) != '\n')
        s[i++] = c;

    s[i] = '\0';

    return i;
}
//char* x;検閲語
//char* s;全体の文字列
//char* ss;全体の文字列＋初めと終わりに空白
//size;全体の文字列の長さ
//len;検閲語の長さ
int check(char *x, char *s, int len, int size) {
    //全体の文字列のはじめと終わりにスペースを設ける
    char ss[55];
    ss[0] = ss[size + 1] == ' ';
    for(int z = 0; z < size; z++) {
        s[z] == ss[z + 1];
    }

    for(int i = 0; i < size; i++) {
        if(ss[i] == ' ') {
            char dst[15];
            //空白があったら'検閲後の長さ＋前後の空白'分の文字列を抜き出す。
            strncpy(dst, ss + i, len + 2);
            dst[len + 2] = '\x0';

            //抜き出した文字列が単語であることの確認
            if(dst[0] == dst[len + 1] == ' ') {
                //一致していなかったら次へ
                for(int t = 0; t < len; t++) {
                    if(dst[t + 1] != x[t] && x[t] != '*') {
                        break;
                    }
                }

                //一致していたら文字列ｓでのその部分を＊にする
                //ssの０番目が空白になっていることに注意
                //ssのi番目は空白であることに注意
                for(int q = i; q < i + len; q++) {
                    s[q] = '*';
                }
            }
        }
    }
    return 0;
}

int main(void) {
    const int num = 60;
    char s[num];

    //getstring関数の実行と入力された文字列の長さの取得
    int size = getstring(s, num);

    int a, b1, c1, d1, e1, f1;
    char b[20], c[20], d[20], e[20], f[20];
    scanf("%d", &a);
    scanf("%s", &b);
    b1 = strlen(b);
    check(b, s, b1, size);
    if(a >= 2) {
        scanf("%s", &c);
        c1 = strlen(c);
        check(c, s, c1, size);
    }
    if(a >= 3) {
        scanf("%s", &d);
        d1 = strlen(d);
        check(d, s, d1, size);
    }
    if(a >= 4) {
        scanf("%s", &e);
        e1 = strlen(e);
        check(e, s, e1, size);
    }
    if(a == 5) {
        scanf("%s", &f);
        f1 = strlen(f);
        check(f, s, f1, size);
    }
    //フィルタリング後の文字列を出力
    printf("%s", s);
    return 0;
}