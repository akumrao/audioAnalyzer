//=======================================================================
/** @file AudioFile.cpp
 *  @author Arvind Umrao
 *  @copyright Copyright (C) 2019  Arvind Umrao
*/
//=======================================================================

#ifndef _AS_AudioFile_h
#define _AS_AudioFile_h

#include <iostream>
#include <vector>
#include <assert.h>
#include <string>
#include "../plot/plot.h"

//=============================================================
/** The different types of audio file, plus some other types to 
 * indicate a failure to load a file, or that one hasn't been
 * loaded yet
 */
enum class AudioFileFormat
{
    Error,
    NotLoaded,
    Wave,
    Aiff
};


// WAVE PCM soundfile format (you can find more in https://ccrma.stanford.edu/courses/422/projects/WaveFormat/ )
typedef struct header_file
{
    char chunk_id[4];  //// 'RIFF'
    int32_t chunk_size;//// make sure to read only 4 bits
    char format[4]; //// 'WAVE'
    char subchunk1_id[4];//// 'fmt'
    int32_t subchunk1_size;
    int16_t audio_format;/*0x0001	WAVE_FORMAT_PCM	PCM
0x0003	WAVE_FORMAT_IEEE_FLOAT	IEEE float
0x0006	WAVE_FORMAT_ALAW	8-bit ITU-T G.711 A-law
0x0007	WAVE_FORMAT_MULAW	8-bit ITU-T G.711 µ-law
0xFFFE	WAVE_FORMAT_EXTENSIBLE	Determined by SubFormat
                          **/
    int16_t num_channels;
    int32_t sample_rate;			// sample_rate denotes the sampling rate.
    int32_t byte_rate;
    int16_t block_align;
    int16_t bits_per_sample;
		// subchunk2_size denotes the number of samples.
} header;

/*

In the wav File, 8-bit samples are stored as unsigned bytes, ranging from 0 to 255. The 16-bit samples are stored as signed integers in 2's-complement.
 
 */

//Read sub chuck ( data or list) , read until data)
/* header of wave file */
typedef struct
{
    char subchunk2_id[4];
    int32_t subchunk2_size;	
} CHUNK_HDR;



using namespace std;
//=============================================================
template <class T>
class AudioFile
{
public:
    
    //=============================================================
    typedef std::vector<std::vector<T> > AudioBuffer;
    
    //=============================================================
    /** Constructor */
    AudioFile();
        
    //=============================================================
    /** Loads an audio file from a given file path.
     * @Returns true if the file was successfully loaded
     */
    bool load (std::string filePath);
    
    bool openWave (std::string filePath);
    
    bool analyzeWave();
    
    /** Saves an audio file to a given file path.
     * @Returns true if the file was successfully saved
     */
    bool save (std::string filePath, AudioFileFormat format = AudioFileFormat::Wave);
        
    //=============================================================
    /** @Returns the sample rate */
    uint32_t getSampleRate() const;
    
    /** @Returns the number of audio channels in the buffer */
    int getNumChannels() const;

    /** @Returns true if the audio file is mono */
    bool isMono() const;
    
    /** @Returns true if the audio file is stereo */
    bool isStereo() const;
    
    /** @Returns the bit depth of each sample */
    int getBitDepth() const;
    
    /** @Returns the number of samples per channel */
    int getNumSamplesPerChannel() const;
    
    /** @Returns the length in seconds of the audio file based on the number of samples and sample rate */
    double getLengthInSeconds() const;
    
    /** Prints a summary of the audio file to the console */
    void printSummary() const;
    
    //=============================================================
    
    /** Set the audio buffer for this AudioFile by copying samples from another buffer.
     * @Returns true if the buffer was copied successfully.
     */
    bool setAudioBuffer (AudioBuffer& newBuffer);
    
    /** Sets the audio buffer to a given number of channels and number of samples per channel. This will try to preserve
     * the existing audio, adding zeros to any new channels or new samples in a given channel.
     */
    void setAudioBufferSize (int numChannels, int numSamples);
    
    /** Sets the number of samples per channel in the audio buffer. This will try to preserve
     * the existing audio, adding zeros to new samples in a given channel if the number of samples is increased.
     */
    void setNumSamplesPerChannel (int numSamples);
    
    /** Sets the number of channels. New channels will have the correct number of samples and be initialised to zero */
    void setNumChannels (int numChannels);
    
    /** Sets the bit depth for the audio file. If you use the save() function, this bit depth rate will be used */
    void setBitDepth (int numBitsPerSample);
    
    /** Sets the sample rate for the audio file. If you use the save() function, this sample rate will be used */
    void setSampleRate (uint32_t newSampleRate);
    
    //=============================================================
    /** A vector of vectors holding the audio samples for the AudioFile. You can 
     * access the samples by channel and then by sample index, i.e:
     *
     *      samples[channel][sampleIndex]
     */
    AudioBuffer samples;
        std::FILE* infile;
private:
    
    //=============================================================
    enum class Endianness
    {
        LittleEndian,
        BigEndian
    };
    
    //=============================================================
    AudioFileFormat determineAudioFileFormat (std::vector<uint8_t>& fileData);
    
    public:
    bool decodeWaveFile (std::vector<uint8_t>& fileData);
    private:
    bool decodeAiffFile (std::vector<uint8_t>& fileData);
    
    //=============================================================
    bool saveToWaveFile (std::string filePath);
    bool saveToAiffFile (std::string filePath);
    
    //=============================================================
    void clearAudioBuffer();
    
    //=============================================================
    int32_t fourBytesToInt (std::vector<uint8_t>& source, int startIndex, Endianness endianness = Endianness::LittleEndian);
    int16_t twoBytesToInt (std::vector<uint8_t>& source, int startIndex, Endianness endianness = Endianness::LittleEndian);
    int getIndexOfString (std::vector<uint8_t>& source, std::string s);
    
    //=============================================================
    T sixteenBitIntToSample (int16_t sample);
    int16_t sampleToSixteenBitInt (T sample);
    
    //=============================================================
    uint8_t sampleToSingleByte (T sample);
    T singleByteToSample (uint8_t sample);
    
    uint32_t getAiffSampleRate (std::vector<uint8_t>& fileData, int sampleRateStartIndex);
    bool tenByteMatch (std::vector<uint8_t>& v1, int startIndex1, std::vector<uint8_t>& v2, int startIndex2);
    void addSampleRateToAiffData (std::vector<uint8_t>& fileData, uint32_t sampleRate);
    T clamp (T v1, T minValue, T maxValue);
    
    //=============================================================
    void addStringToFileData (std::vector<uint8_t>& fileData, std::string s);
    void addInt32ToFileData (std::vector<uint8_t>& fileData, int32_t i, Endianness endianness = Endianness::LittleEndian);
    void addInt16ToFileData (std::vector<uint8_t>& fileData, int16_t i, Endianness endianness = Endianness::LittleEndian);
    
    //=============================================================
    bool writeDataToFile (std::vector<uint8_t>& fileData, std::string filePath);
    
    //=============================================================
    
    
   //=============================================================
    bool play ();
    
    //=============================================================
    
    
    AudioFileFormat audioFileFormat;
    uint32_t sampleRate;
    int bitDepth;
    int nChannels;
    int totalSamplesPerChannel;
    struct header_file  meta;

    public:

    plot_params *params1; 
};

#endif /* AudioFile_h */
