/*
	@file	AudioMaster.h
	@date	2019/04/18
	@author	番場 宥輝
	@brief	オーディオデバイスクラス
	@detail	XAudioを使ったAudioデバイスを管理
			シングルトン
*/
#pragma once
#pragma comment(lib,"Xaudio2.lib")

#include <XAudio2.h>
#include "Singleton.h"

/*! Direct3D11関連 */
namespace D3D11 {

	/*! サウンド関連の名前空間に含める */
	namespace Sound {

		/*!
			@brief	オーディオのデバイスクラス
		*/
		class AudioMaster
			:public Singleton<AudioMaster>
		{
		public:
			/*!
				@brief	デストラクタ
			*/
			~AudioMaster();

			/*!
				@fn		Initialize
				@brief	初期化
				@return	S_OK:成功 E_FAIL:失敗
			*/
			HRESULT Initialize();

			/*!
				@fn		Finalize
				@brief	破棄処理
				@detail	メンバの明示的な解放とメンバの初期化
			*/
			void Finalize();

			/*!
				@fn		GetXAudio2
				@brief	オーディオデバイスの取得
				@return	オーディオデバイス
			*/
			IXAudio2* GetXAudio2() { return m_pXAudio2; }

			/*!
				@fn		GetMasterVoice
				@brief	マスタリングボイスの取得
				@return	マスタリングボイス
			*/
			IXAudio2MasteringVoice* GetMasterVoice() { return m_pMasteringVoice; }
		private:
			/*!
				@brief	コンストラクタ
			*/
			AudioMaster();

			/*!
				@brief	シングルトンデザインパターンのテンプレート継承
			*/
			friend class Singleton<AudioMaster>;

			/*!
				@var	m_pXAudio2
				@brief	オーディオデバイス
			*/
			IXAudio2*					m_pXAudio2;

			/*!
				@var	m_pMasteringVoice
				@brief	マスタリングボイス
			*/
			IXAudio2MasteringVoice*		m_pMasteringVoice;
		};

	}
}
