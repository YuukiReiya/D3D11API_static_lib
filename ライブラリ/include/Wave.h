/*
	@file	Wave.h
	@date	2018/02/11
	@author	番場 宥輝
	@brief	Waveサウンド
	@detail	.wav拡張子の音声再生
*/
#pragma once
#include <string>
#include <XAudio2.h>

/*! API名前空間に含める */
namespace API {

	class Wave
	{
	public:
		/*!
			@brief	コンストラクタ
		*/
		explicit Wave();

		/*!
			@brief	デストラクタ
		*/
		~Wave();

		/*!
			@fn		イニシャライズ
			@brief	初期化
			@detail	セキュアゼロメモリ
			@return	S_OK:成功 E_FAIL:失敗
		*/
		HRESULT Initialize();

		/*!
			@fn		ファイナライズ
			@brief	破棄処理
			@detail	メンバの明示的な解放
		*/
		void Finalize();

		/*!
			@fn			ロード
			@brief		ファイルのロード
			@detail		Waveファイル読み込み
			@param[in]	読み込むファイルのパス
			@return		true:成功 false:失敗
		*/
		bool Load(std::string filePath);

		/*!
			@fn			プレイ
			@brief		音の再生
			@detail		一時停止、停止後の"再生"もこれを使う
			@param[in]	ループフラグ
		*/
		void Play(bool isLoop = false);

		/*!
			@fn		ストップ
			@brief	音の停止
		*/
		void Stop();

		/*!
			@fn		ポーズ
			@brief	音の一時停止
		*/
		void Pause();

		/*!
			@fn			ボリュームのセット
			@brief		音の音量を設定
			@detail		範囲 0～1
			@param[in]	設定する音量
		*/
		void SetVolume(float vol);
	private:
		IXAudio2SourceVoice* m_pSourceVoice;
		BYTE* m_pWaveBuffer;
		DWORD m_dwWaveSize;
	};

};
