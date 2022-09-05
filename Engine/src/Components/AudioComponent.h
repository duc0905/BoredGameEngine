#pragma once
#include"Component.h"
#include"../pch.h"
#include"../Audio/ISoundBuffer.h"
#include "../Audio/OpenALAudio.h"

/**
* Audio Component that can be used in Actor to read and play customized sounds
* from resources directory
*/
class AudioComponent:
	public Component
{
private:
	// source of sound to be played
	std::shared_ptr<ISource> source_;

	// buffer use for source to play sound
	std::shared_ptr<ISoundBuffer> soundBuffer_;

public:
	/**
	* Class Contructor
	*/
	AudioComponent();

	/**
	* Class Destrutor
	*/
	~AudioComponent();

	/**
	* Play sound if the sound is set in source
	*/
	void PlayCompSound();
	
	/**
	* Set gain in the source (Volume control)
	*/
	void SetGain(float gain);

	/**
	* Set sound in source to a specific wav file in resources directory
	* 
	* @args: const std::string file - name of the wav file using
	*/
	void SetSound(const std::string& file);

	/**
	* Set sound in source to a specific wav file in resources directory
	* 
	* @args: std::shared_ptr<ISoundBuffer> - name of the sound buffer using
	*/
	void SetSound(std::shared_ptr<ISoundBuffer> buffer);

	/**
	* Set the position of the sound source	
	* 
	* @args: const glm::vec3& pos - New position vector
	*/
	// TODO: research this design more
	void SetPosition(const glm::vec3& pos);

	const std::string& GetComponentName();
	void Update(double dt) {};
};

