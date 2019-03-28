/*
	@file	Wave.h
	@date	2018/02/11
	@author	番場 宥輝
	@brief	Waveサウンド
*/
#include "stdafx.h"
#include "Wave.h"
#include "MyGame.h"
#include "MemoryLeaks.h"
#include "AudioMaster.h"

/*!
	@brief	名前空間
	@detail	usingディレクティブ
*/
using namespace API;
using namespace D3D11::Sound;

/*!
	@brief	コンストラクタ
*/
Wave::Wave()
{
	Initialize();
}

/*!
	@brief	デストラクタ
*/
Wave::~Wave()
{
	Finalize();
}

/*!
	@fn		イニシャライズ
	@brief	初期化
	@detail	セキュアゼロメモリ
	@return	S_OK:成功 E_FAIL:失敗
*/
HRESULT Wave::Initialize()
{
	SecureZeroMemory(this, sizeof(this));
	return S_OK;
}

/*!
	@fn			ロード
	@brief		ファイルのロード
	@detail		Waveファイル読み込み
	@param[in]	読み込むファイルのパス
	@return		true:成功 false:失敗
*/
bool Wave::Load(std::string filePath)
{
	HMMIO hMmio = NULL;				// WindowsマルチメディアAPIハンドル
	DWORD waveSize = 0;				// Waveデータサイズ
	WAVEFORMATEX*	pFormat;		// Waveフォーマット
	MMCKINFO		chunkInfo;		// チャンク情報
	MMCKINFO		riffChunkInfo;	// 最上部チャンク
	PCMWAVEFORMAT	pcmFormat;		// PCMフォーマット

	// Waveファイル内のヘッダー情報読み込み
	auto s_path = To_TString(filePath);
	const auto path = const_cast<LPTSTR>(s_path.c_str());
	hMmio = mmioOpen(path, NULL, MMIO_ALLOCBUF | MMIO_READ);

	// Waveファイルの読み込み失敗
	if (hMmio == NULL) {
		std::string error = "\"" + filePath + "\" is not load in sound!";
		ErrorLog(error);
		return false;
	}

	// ファイルポインタをRIFFチャンクの先頭にセット
	mmioDescend(hMmio, &riffChunkInfo, NULL, 0);

	// ファイルポインタを'f' 'm' 't' ' ' チャンクにセットする
	chunkInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hMmio, &chunkInfo, &riffChunkInfo, MMIO_FINDCHUNK);

	// フォーマット読み込み
	mmioRead(hMmio, (HPSTR)&pcmFormat, sizeof(pcmFormat));
	pFormat = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
	memcpy(pFormat, &pcmFormat, sizeof(pcmFormat));
	pFormat->cbSize = 0;
	mmioAscend(hMmio, &chunkInfo, 0);

	// 音データ読み込み
	chunkInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hMmio, &chunkInfo, &riffChunkInfo, MMIO_FINDCHUNK);
	waveSize = chunkInfo.cksize;
	m_pWaveBuffer = new BYTE[waveSize];
	DWORD offset = chunkInfo.dwDataOffset;
	mmioRead(hMmio, (char*)m_pWaveBuffer, waveSize);

	// オーディオデバイスの参照
	auto& manager = AudioMaster::GetInstance();

	// ソースボイスにデータ詰め込み
	XAUDIO2_SEND_DESCRIPTOR sendDescriptor;
	sendDescriptor.Flags = XAUDIO2_SEND_USEFILTER;
	sendDescriptor.pOutputVoice = manager.GetMasterVoice();
	const XAUDIO2_VOICE_SENDS sendList = { 1,&sendDescriptor };

	// ソースボイス作成
	HRESULT hr = manager.GetXAudio2()->CreateSourceVoice(&m_pSourceVoice, pFormat, 0, 2.0f, NULL, &sendList);

	SafeDelete(pFormat);

	if (FAILED(hr)) {
		ErrorLog("SourceVoice is not create!");
		return false;
	}
	m_dwWaveSize = waveSize;

	return true;
}

/*!
	@fn		ファイナライズ
	@brief	破棄処理
	@detail	メンバの明示的な解放
*/
void Wave::Finalize()
{
	SafeDelete(m_pWaveBuffer);
}

/*!
	@fn			プレイ
	@brief		音の再生
	@detail		一時停止、停止後の"再生"もこれを使う
	@param[in]	ループフラグ
*/
void Wave::Play(bool isLoop)
{
	// サブミット
	XAUDIO2_BUFFER buffer = { 0 };
	SecureZeroMemory(&buffer, sizeof(buffer));
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = m_dwWaveSize;
	buffer.pAudioData = m_pWaveBuffer;
	buffer.PlayBegin = 0;	// 再生されるバッファの最初のサンプル
	buffer.PlayLength = 0;	// サウンドのバッファ全て
	if (isLoop) {

		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	else {

		buffer.LoopBegin = XAUDIO2_NO_LOOP_REGION;
		buffer.LoopCount = 0;
	}
	HRESULT hr = m_pSourceVoice->SubmitSourceBuffer(&buffer);
	if (FAILED(hr)) {
		std::string error = "Wave is not played!";
		ErrorLog(error);
		return;
	}
	m_pSourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
}

/*!
	@fn		ストップ
	@brief	音の停止
*/
void Wave::Stop()
{
	m_pSourceVoice->Stop(0, XAUDIO2_COMMIT_NOW);
	m_pSourceVoice->FlushSourceBuffers();
}

/*!
	@fn		ポーズ
	@brief	音の一時停止
*/
void API::Wave::Pause()
{
	m_pSourceVoice->Stop(0, XAUDIO2_COMMIT_NOW);
}


/*!
	@brief		音量変更
	@detail		範囲 0～1
	@param[in]	設定する音量
*/
void API::Wave::SetVolume(float vol)
{
	m_pSourceVoice->SetVolume(vol);
}