#include "audio_loader.h"


AudioLoader::AudioLoader()
{}


void AudioLoader::allUpdate()
{
	for( auto& itr : container_map_ )
		itr.second.get()->engineUpdate();
}

AudioContainer* AudioLoader::getSound( std::wstring FileName )
{
	auto checker = container_map_.find( FileName );

	if( checker != container_map_.end() )
		return checker->second.get();

	AudioContainer* p_container = new AudioContainer( FileName );

	container_map_.insert( std::make_pair( FileName , std::move( p_container ) ) );

	return p_container;
}

void AudioLoader::desetroySound( std::wstring FileName )
{
	container_map_.erase( FileName );
}

void AudioLoader::allDestroy()
{
	container_map_.clear();
}