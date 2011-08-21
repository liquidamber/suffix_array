/* 
 * gen_prob.h
 */

#pragma once

#ifndef __GEN_PROB_H__
#define __GEN_PROB_H__

typedef struct problem {
  int problem_type;		/* 問題のタイプ(番号) */
  int alphabet_size;		/* 文字の種類 <= 96 */
  int reference_string_len;	/* 参照文字列の長さ */
  int n_query_strings;		/* 問い合わせ文字列数 */
  int max_query_string_len;	/* 一つの問い合わせ文字列の最大長 */
  int max_hits_to_answer;       /* 出現位置を回答すべき文字列の出現回数の最大値(下記(*)参照) */
  char * alphabets;		/* 文字の集合 */
  char * reference_string;	/* 参照文字列 */
  char ** query_strings;	/* 問い合わせ文字列の配列 */
} problem, * problem_t;

typedef struct answer {
  int n_hits;			/* 出現した回数 */
  int * hit_indices;		/* 出現位置の配列. 
				   (*)ただし, n_hitsが上記のmax_hits_to_answerを超える場合は,
				   これは何でも良い(n_hitsだけが正しければOK) */
} answer, * answer_t;

enum {
  problem_type_practice = 0,
  problem_type_malei = 90,
  problem_type_real_begin = 100,
  problem_type_real_end = 114,
};


/* 
 * [1] 問題を生成する関数:
 */

problem_t gen_problem(int problem_type,
		      int alphabet_size, 
		      int reference_string_len,
		      int n_query_strings,
		      int max_query_string_len);
/* 
   引数:

   problem_type:         練習用は0. 現在は0以外を与えるとエラーで終了.
                         本番用は後日指定する.
   alphabet_size:        使ってほしい文字の種類数. 
   reference_string_len: 生成してほしい参照文字列の長さ.
   n_query_strings:      生成してほしい問い合わせ文字列の数.
   max_query_string_len: 一つの問い合わせ文字列の最大長を指定.

   alphabet_size, reference_string_len, n_query_strings,
   max_query_string_lenはいずれも0でもよく, その場合,システムが「お試し
   用」のデフォルト値を選ぶ. 使われた値は返された構造体中の同じ名前の
   フィールドに書かれている.
   最も簡便な呼び出し方は以下のとおり:
   problem_t p = gen_problem(0, 0, 0, 0, 0);

   返り値:

   problem構造体へのポインタ (problem_t).
 */

/* 
 * [2] 問題データ (problem 構造体)の形式
 */
 
/*
  problem_type, alphabet_size, reference_string_len, n_query_strings,
  max_query_string_lenの意味は, gen_problemsのものと同じ.
  その他のフィールドの形式は以下のとおり.

  reference_string :  ヌル文字(0)で終了する参照文字列.
  query_strings :     query_strings[0]から,
                      query_strings[n_query_strings-1]までが
		      問い合わせ文字列. 各問い合わせ文字列は,
		      ヌル文字で終了する.
		      query_strings[n_query_strings]には NULL が入っている.
  max_hits_to_answer: 出現位置を答えなくてはならない問い合わせ文字列
                      の出現回数の最大値. 例えばこれが10000だったとすると,
		      解答プログラムは出現回数が10000以下の問い合わせ文字列
		      に対しては, 出現位置をすべて答える. それよりも多く
		      (10001回以上)出現した文字列については, 出現回数
		      のみを答えればよく, 出現位置の配列を答える必要はない.
		      (メモリを確保するときにこの情報を使えば,
		      プログラムは多少簡単になるだろう).

  そして, 解答作成に当たっては, 以下を仮定して良い.

    max_hits_to_answer * 問い合わせ文字列数 * 4 <= 参照文字列の長さ

  端的に言って, 解答に必要な出現位置の配列は, 全部合わせて,
  参照文字列の長さ程度のメモリしか必要としない, ということである.
  (例えば 100M 文字の参照文字列であれば, 解答のために必要なメモリも
  最大で100Mバイト(程度)という意味.

 */


/* 
 * [3] 解答データ (answer構造体)の形式
 */

/* 
   answer構造体の配列を作り, その i 番目の要素に,
   i 番目の問い合わせ文字列に対する答えを格納する.

   一つの問い合わせ文字列に対する答えは以下の二つのフィールドからなる.
     - n_hits:        その問い合わせ文字列の出現回数
     - n_hit_indices: 出現位置(整数)のリスト. 
                      ただし, 数字の小さい順に整列されていること.
                      そして n_hits が上述の max_hits_to_answer (problem構造体中
		      の要素)を超える場合, この配列は何でも良い(チェックされない).

たとえば, 

   reference_string = "momoko taura. nanako taura. desu",
   query_strings = { "mo", "o" };
   max_hits_to_answer = 4;

とすると, 答えは以下のようになっているべきである.

   ans[0].n_hits = 2
   ans[0].hit_indices = { 0, 2 };
   ans[1].n_hits = 4
   ans[1].hit_indices = { 1, 3, 5, 19 };

仮に max_hits_to_answer = 2 だったとすると, ans[1].hit_indicesはなんでもよい.

   ans[0].n_hits = 2
   ans[0].hit_indices = { 0, 2 };
   ans[1].n_hits = 4
   ans[1].hit_indices = なんでもよい. NULLでもよい.


 */

/* 
 * [4] 解答をチェックする関数 
 */

int check_answer(problem_t prob, answer_t ans);

/* 
   引数:
   prob : gen_problem の返り値である, 問題データへのポインタ
   ans : 全問い合わせ文字列の解答がそろった配列

   返り値:
   すべて正解なら1, 誤答が一つでもあれば0.

   誤答があった場合, 何番目のデータで誤答があったかが表示される.
 */


/* 
 * [5] デバッグ用の親切関数
 */

answer_t tell_me_answer(problem_t prob);

/* 
   引数: gen_problemの返り値である, 問題データへのポインタ
   返り値: 解答

   これは, 自分のプログラムの答えのどこがどう間違っているのかを
   調べるために, 参考用として提供するものである.
   もちろんこれを利用して正解を出すなどというのはルール違反.
   (check_answerは, 一度でもこれを呼んだら, 提出された解答を
   無効と扱うように書かれているが, 出し抜く手段がないわけでは
   ないだろう. そんな方法を思いついても実行しないように)

   なお, この関数はさしあたり, 練習用(problem_type = 0)に
   大して提供されているが今後のデータに対しても提供されるか,
   保証はしない. 
 */



/* 
   使い方
   [基本] 
   (1) 自分のプログラム中で, gen_problem を呼ぶ.
   返り値は problem構造体へのポインタ.
   (2) 自分のプログラムで, query_stringsの各要素(文字列)が,
   reference_string 中にどれだけ現れるかを求める.
   求めた値は, answer 構造体に格納する.
   (3) すべての解答が揃ったら, check_answerを呼び出す.
   以下がテンプレート:
 */

#if 0

#include "gen_prob.h"

int main() {
  // データを生成
  problem_t p = gen_problem( ... );
  answer_t ans = (answer_t)malloc(sizeof(answer) * p->n_query_strings);
  int i;
  for (i = 0; i < p->n_query_strings; i++) {
    参照文字列(p->reference_string)から, 
      i番目の問い合わせ文字列(p->query_strings[i])を検索.
    // ans[i]に答えを収納.
    ans[i].n_hits = n;  // 何個見つかったか
    if (n <= p->max_hits_to_answer) {
      ans[i].hit_indices = ...;  // それらのindexを小さい順に格納した配列
    } else {
      ans[i].hit_indices = NULL; // 不要だが一応
    }
  }
  // すべての解答がそろったらチェック
  check_answer(p, ans);
}

#endif	/* 0 */



#endif
