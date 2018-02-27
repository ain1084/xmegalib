/*
 * AudioPlayerCreator.h
 *
 * Created: 2011/12/28 1:58:41
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

namespace FileSystem
{
	class IFile;
}

namespace Audio
{
	class ISampleRenderer;
	class IPlayer;
}

///入力ファイルのフォーマットに対応する AudioPlayer のインスタンスを構築する。
class AudioPlayerCreator
{
public:
	///コンストラクタ
	///
	///AudioPlayerCreator のコンストラクタ。
	///
	///指定された引数で AudioPlayerCreator を構築する。
	///	
	///@param[in] renderer Player のレンダリング先を指定する
	AudioPlayerCreator(Audio::ISampleRenderer& renderer);

	///ファイルに対応する IAudioPlayer インスタンスを生成して返す
	///
	///開かれた File で示されるファイルの内容から対応する IAudioPlayer インスタンスを生成する。
	///
	///@param[in] file 開かれている File
	///@return 対応する IAudioPlayer のインスタンス。NULL ならば対応 Player が無かった事を意味する。
	///        返された IAudioPlayer インスタンスの削除は呼び出し側で行うこと。
	Audio::IPlayer* CreateAudioPlayer(FileSystem::IFile& file);

private:
	///レンダラー(出力先)を保持する
	Audio::ISampleRenderer& _renderer;
};
