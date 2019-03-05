/*
	@file	Singleton.h
	@date	2018/11/10
	@author	番場 宥輝
	@brief	シングルトンクラスの基底クラス
	@detail	テンプレートを使ったメタプログラミング
			参考:https://qiita.com/narumi_/items/b205e59e7fc81695f380
*/
#pragma once

template<class T>
class Singleton {

public:

	// インスタンスのゲッター
	static inline T& GetInstance() {
		static T instance;
		return instance;
	}

protected:

	// 空コンストラクタ
	Singleton() {}			// 外部でSingletonクラスのインスタンス作成を禁止する

	// 仮想デストラクタ
	virtual ~Singleton() {}	// このクラス自体にメンバは持っていないので特にやることもないし空で定義しておく

private:

	// 代入オペレーター削除
	void operator=(const Singleton&) = delete;

	// コピーコンストラクタ削除
	Singleton(const Singleton&) = delete;
};