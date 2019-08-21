//=======================================================================
/** @file AudioFile.cpp
 *  @author Arvind Umrao
 *  @copyright Copyright (C) 2019  Arvind Umrao
 */
/*
 *    
 * error_printf("Unsigned input only supported with bitwidth 8\n");
 * 8-bit samples are stored as unsigned bytes, ranging from 0 to 255. 16-bit samples are stored as 2's-complement signed integers, ranging from -32768 to 32767.
 * http://soundfile.sapp.org/doc/WaveFormat/
https://www.ee.iitb.ac.in/student/~daplab/resources/wav_read_write.cpp
https://github.com/dilawar/sound/blob/master/src/wav-file.cc
https://github.com/jwhu1024/pcm-to-wav/blob/master/src/wave.c
https://github.com/haloboy777/wav-to-pcm
https://superuser.com/questions/597227/linux-arecord-capture-sound-card-output-rather-than-microphone-input
//graph
https://dilawarnotes.wordpress.com/2010/10/15/opening-wav-file-using-cc/
https://carthick.wordpress.com/2007/11/26/linux-recording-soundcard-output-using-arecord/
 */
#include "AudioFile.h"
#include <fstream>
#include <unordered_map>
#include <iterator>
#include <sstream>
#include <cstring> 
#include <iostream> 

#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <numeric> // std::iota 
#include <climits> 

 


//=============================================================
// Pre-defined 10-byte representations of common sample rates
std::unordered_map <uint32_t, std::vector<uint8_t>> aiffSampleRateTable = {
    {8000,
        {64, 11, 250, 0, 0, 0, 0, 0, 0, 0}},
    {11025,
        {64, 12, 172, 68, 0, 0, 0, 0, 0, 0}},
    {16000,
        {64, 12, 250, 0, 0, 0, 0, 0, 0, 0}},
    {22050,
        {64, 13, 172, 68, 0, 0, 0, 0, 0, 0}},
    {32000,
        {64, 13, 250, 0, 0, 0, 0, 0, 0, 0}},
    {37800,
        {64, 14, 147, 168, 0, 0, 0, 0, 0, 0}},
    {44056,
        {64, 14, 172, 24, 0, 0, 0, 0, 0, 0}},
    {44100,
        {64, 14, 172, 68, 0, 0, 0, 0, 0, 0}},
    {47250,
        {64, 14, 184, 146, 0, 0, 0, 0, 0, 0}},
    {48000,
        {64, 14, 187, 128, 0, 0, 0, 0, 0, 0}},
    {50000,
        {64, 14, 195, 80, 0, 0, 0, 0, 0, 0}},
    {50400,
        {64, 14, 196, 224, 0, 0, 0, 0, 0, 0}},
    {88200,
        {64, 15, 172, 68, 0, 0, 0, 0, 0, 0}},
    {96000,
        {64, 15, 187, 128, 0, 0, 0, 0, 0, 0}},
    {176400,
        {64, 16, 172, 68, 0, 0, 0, 0, 0, 0}},
    {192000,
        {64, 16, 187, 128, 0, 0, 0, 0, 0, 0}},
    {352800,
        {64, 17, 172, 68, 0, 0, 0, 0, 0, 0}},
    {2822400,
        {64, 20, 172, 68, 0, 0, 0, 0, 0, 0}},
    {5644800,
        {64, 21, 172, 68, 0, 0, 0, 0, 0, 0}}
};

//=============================================================

template <class T>
AudioFile<T>::AudioFile():alradyPlayed(0), m_Soppped(false)  {
    bitDepth = 16;
    sampleRate = 44100;
    samples.resize(1);
    samples[0].resize(0);
    audioFileFormat = AudioFileFormat::NotLoaded;
}

//=============================================================

template <class T>
uint32_t AudioFile<T>::getSampleRate() const {
    return sampleRate;
}

//=============================================================

template <class T>
int AudioFile<T>::getNumChannels() const {
    return nChannels;
}

//=============================================================

template <class T>
bool AudioFile<T>::isMono() const {
    return getNumChannels() == 1;
}

//=============================================================

template <class T>
bool AudioFile<T>::isStereo() const {
    return getNumChannels() == 2;
}

//=============================================================

template <class T>
int AudioFile<T>::getBitDepth() const {
    return bitDepth;
}

//=============================================================

template <class T>
int AudioFile<T>::getNumSamplesPerChannel() const {
    if (samples.size() > 0)
        return (int) samples[0].size();
    else
        return 0;
}

//=============================================================

template <class T>
double AudioFile<T>::getLengthInSeconds() const {
    return (double) getNumSamplesPerChannel() / (double) sampleRate;
}

//=============================================================

template <class T>
void AudioFile<T>::printSummary() const {
    std::cout << "|======================================|" << std::endl;
    std::cout << "Num Channels: " << getNumChannels() << std::endl;
    std::cout << "Num Samples Per Channel: " << getNumSamplesPerChannel() << std::endl;
    std::cout << "Sample Rate: " << sampleRate << std::endl;
    std::cout << "Bit Depth: " << bitDepth << std::endl;
    std::cout << "Format 1:Int,3:Float" << audioFormat << std::endl;
    std::cout << "Length in Seconds: " << getLengthInSeconds() << std::endl;
    std::cout << "|======================================|" << std::endl;
}

//=============================================================

template <class T>
bool AudioFile<T>::setAudioBuffer(AudioBuffer& newBuffer) {
    int numChannels = (int) newBuffer.size();

    if (numChannels <= 0) {
        assert(false && "The buffer your are trying to use has no channels");
        return false;
    }

    int numSamples = (int) newBuffer[0].size();

    // set the number of channels
    samples.resize(newBuffer.size());

    for (int k = 0; k < getNumChannels(); k++) {
        assert(newBuffer[k].size() == numSamples);

        samples[k].resize(numSamples);

        for (int i = 0; i < numSamples; i++) {
            samples[k][i] = newBuffer[k][i];
        }
    }

    return true;
}

//=============================================================

template <class T>
void AudioFile<T>::setAudioBufferSize(int numChannels, int numSamples) {
    samples.resize(numChannels);
    setNumSamplesPerChannel(numSamples);
}

//=============================================================

template <class T>
void AudioFile<T>::setNumSamplesPerChannel(int numSamples) {
    int originalSize = getNumSamplesPerChannel();

    for (int i = 0; i < getNumChannels(); i++) {
        samples[i].resize(numSamples);

        // set any new samples to zero
        if (numSamples > originalSize)
            std::fill(samples[i].begin() + originalSize, samples[i].end(), (T) 0.);
    }
}

//=============================================================

template <class T>
void AudioFile<T>::setNumChannels(int numChannels) {
    int originalNumChannels = nChannels;
    int originalNumSamplesPerChannel = getNumSamplesPerChannel();
    nChannels = numChannels;
    samples.resize(numChannels);

    // make sure any new channels are set to the right size
    // and filled with zeros
    if (numChannels > originalNumChannels) {
        for (int i = originalNumChannels; i < numChannels; i++) {
            samples[i].resize(originalNumSamplesPerChannel);
            std::fill(samples[i].begin(), samples[i].end(), (T) 0.);
        }
    }
}

//=============================================================

template <class T>
void AudioFile<T>::setBitDepth(int numBitsPerSample) {
    bitDepth = numBitsPerSample;
}

//=============================================================

template <class T>
void AudioFile<T>::setSampleRate(uint32_t newSampleRate) {
    sampleRate = newSampleRate;
}

//=============================================================

template <class T>
bool AudioFile<T>::load(std::string filePath) {
    std::ifstream file(filePath, std::ios::binary);

    // check the file exists
    if (!file.good()) {
        std::cout << "ERROR: File doesn't exist or otherwise can't load file" << std::endl;
        std::cout << filePath << std::endl;
        return false;
    }

    file.unsetf(std::ios::skipws);
    std::istream_iterator<uint8_t> begin(file), end;
    std::vector<uint8_t> fileData(begin, end);

    // get audio file format
    audioFileFormat = determineAudioFileFormat(fileData);

    if (audioFileFormat == AudioFileFormat::Wave) {
        return decodeWaveFile(fileData);
    } else if (audioFileFormat == AudioFileFormat::Aiff) {
        return decodeAiffFile(fileData);
    } else {
        std::cout << "Audio File Type: " << "Error" << std::endl;
        return false;
    }
}

template <class T>
bool AudioFile<T>::openWave(std::string filePath) {

    infile = std::fopen(filePath.c_str(), "rb");

    // header_p meta = (header_p)malloc(sizeof(header));	// header_p points to a header struct that contains the wave file metadata fields
    int nb; // variable storing number of byes returned
    if (infile) {
        fread(&meta, 1, sizeof (header), infile);
        //	fwrite(meta,1, sizeof(*meta), outfile);
        cout << " Size of Header file is " << sizeof (meta) << " bytes" << endl;
        cout << " Sampling rate of the input wave file is " << meta.sample_rate << " Hz" << endl;
        // cout << " Number of samples in wave file are " << meta.subchunk2_size << " samples" << endl;




        // if we can't find the data or format chunks, or the IDs/formats don't seem to be as expected
        // then it is unlikely we'll able to read this file, so abort
        if (std::strncmp(meta.format, "WAVE", 4) || std::strncmp(meta.subchunk1_id, "fmt", 3)) {
            std::cout << "ERROR: this doesn't seem to be a valid .WAV file" << std::endl;
            return false;
        }


        //int32_t formatChunkSize = fourBytesToInt (fileData, f + 4);
        audioFormat = meta.audio_format;
        nChannels = meta.num_channels;
        sampleRate = meta.sample_rate;
        int32_t numBytesPerSecond = meta.byte_rate;
        int16_t numBytesPerBlock = meta.block_align;
        bitDepth = meta.bits_per_sample;

        int numBytesPerSample = bitDepth / 8;

        // check that the audio format is PCM
       if (!(audioFormat == 1 || audioFormat == 3)){
            std::cout << "ERROR: this is a compressed .WAV file and this library does not support decoding them at present" << std::endl;
            return false;
        }

        // check the number of channels is mono or stereo
        if (nChannels < 1 || nChannels > 2) {
            std::cout << "ERROR: this WAV file seems to be neither mono nor stereo (perhaps multi-track, or corrupted?)" << std::endl;
            return false;
        }

        // check header data is consistent
        if ((numBytesPerSecond != (nChannels * sampleRate * bitDepth) / 8) || (numBytesPerBlock != (nChannels * numBytesPerSample))) {
            std::cout << "ERROR: the header data in this WAV file seems to be inconsistent" << std::endl;
            return false;
        }

        // check bit depth is either 8, 16 or 24 bit
        if (bitDepth != 8 && bitDepth != 16 && bitDepth != 24 && bitDepth != 32) {
            std::cout << "ERROR: this file has a bit depth that is not 8, 16 or 24 bits" << std::endl;
            return false;
        }


        CHUNK_HDR subchunk;
        int sFlag = 1;
        while (sFlag != 0) {
            // check attempts.
            if (sFlag > 10) {
                std::cout << "\nToo many chunks";
                exit(-1);
            }



            // read chunk header
            //    cerr << "Size of chunk" << sizeof( CHUNK_HDR) << endl;
            int stat = std::fread(&subchunk, sizeof (CHUNK_HDR), 1, infile);
            if (1 != stat) {
                std::cout << "\n I just can't read data. Sorry!";
                exit(-1);
            }


            if (strncmp(subchunk.subchunk2_id, "data", 4) == 0) {
                break;
            }

            // skip over chunk.
            sFlag++;
            stat = std::fseek(infile, subchunk.subchunk2_size, SEEK_CUR);
            if (stat != 0) {
                std::cout << "\nCan't seek.";
                exit(-1);
            }

        }

        /* find length of remaining data. */
        totalSamplesPerChannel = subchunk.subchunk2_size / (nChannels * bitDepth / 8);

        return 1;
    }

}

template <class T>
bool AudioFile<T>::analyzeWave() {

    if (!m_Soppped) {

        static int frameSampleSize = 0;
        process.codecEncodeChunk(rawPCMint16);

        coordlist coordinate_list = NULL;

        params1->clean();
        params1->update = true;
        //coordinate_list = push_back_coords(coordinate_list, 0, &samples[0][0] , 1024);
        //Pair max = coordinate_list->max();
        //Pair min = coordinate_list->min();
        //std::cout <<  max.x  <<  ", " << max.y << " , " << min.x  <<  ", " << min.y << " , "  << std::endl << std::flush;
        // params1->max = max;
        // params1->min = min;
        //clear_coord(coordinate_list);

        params1->push_back(0, &samples[0][frameSampleSize], 1024);


        frameSampleSize += 1024;
        if (frameSampleSize > samples[0].size()) {
            frameSampleSize = 0;
        }


        return true;
    }
    else 
    {
        return false;
    }
}

//=============================================================

template <class T>
bool AudioFile<T>::decodeWaveFile(std::vector<uint8_t>& fileData) {
    // -----------------------------------------------------------
    // HEADER CHUNK
    std::string headerChunkID(fileData.begin(), fileData.begin() + 4);
    //int32_t fileSizeInBytes = fourBytesToInt (fileData, 4) + 8;
    std::string format(fileData.begin() + 8, fileData.begin() + 12);

    // -----------------------------------------------------------
    // try and find the start points of key chunks
    int indexOfDataChunk = getIndexOfString(fileData, "data");
    int indexOfFormatChunk = getIndexOfString(fileData, "fmt");

    // if we can't find the data or format chunks, or the IDs/formats don't seem to be as expected
    // then it is unlikely we'll able to read this file, so abort
    if (indexOfDataChunk == -1 || indexOfFormatChunk == -1 || headerChunkID != "RIFF" || format != "WAVE") {
        std::cout << "ERROR: this doesn't seem to be a valid .WAV file" << std::endl;
        return false;
    }

    // -----------------------------------------------------------
    // FORMAT CHUNK
    int f = indexOfFormatChunk;
    std::string formatChunkID(fileData.begin() + f, fileData.begin() + f + 4);
    //int32_t formatChunkSize = fourBytesToInt (fileData, f + 4);
    audioFormat = twoBytesToInt(fileData, f + 8);
    nChannels = twoBytesToInt(fileData, f + 10);
    sampleRate = (uint32_t) fourBytesToInt(fileData, f + 12);
    int32_t numBytesPerSecond = fourBytesToInt(fileData, f + 16);
    int16_t numBytesPerBlock = twoBytesToInt(fileData, f + 20);
    bitDepth = (int) twoBytesToInt(fileData, f + 22);

    int numBytesPerSample = bitDepth / 8;

    // check that the audio format is PCM
    if (!(audioFormat == 1 || audioFormat == 3)) {
        std::cout << "ERROR: this is a compressed .WAV file and this library does not support decoding them at present" << std::endl;
        return false;
    }

    // check the number of channels is mono or stereo
    if (nChannels < 1 || nChannels > 2) {
        std::cout << "ERROR: this WAV file seems to be neither mono nor stereo (perhaps multi-track, or corrupted?)" << std::endl;
        return false;
    }

    // check header data is consistent
    if ((numBytesPerSecond != (nChannels * sampleRate * bitDepth) / 8) || (numBytesPerBlock != (nChannels * numBytesPerSample))) {
        std::cout << "ERROR: the header data in this WAV file seems to be inconsistent" << std::endl;
        return false;
    }

    // check bit depth is either 8, 16 or 24 bit
    if (bitDepth != 8 && bitDepth != 16 && bitDepth != 24 && bitDepth != 32) {
        std::cout << "ERROR: this file has a bit depth that is not 8, 16, 24 or 32 bits" << std::endl;
        return false;
    }

    // -----------------------------------------------------------
    // DATA CHUNK
    int d = indexOfDataChunk;
    std::string dataChunkID(fileData.begin() + d, fileData.begin() + d + 4);
    int32_t dataChunkSize = fourBytesToInt(fileData, d + 4);

    int numSamples = dataChunkSize / (nChannels * bitDepth / 8);
    int samplesStartIndex = indexOfDataChunk + 8;

    clear();
    samples.resize(nChannels);
    

    rawPCMint16.resize(nChannels);
   
    

    rawPCMInt.assign( &fileData[samplesStartIndex],  &fileData[samplesStartIndex +dataChunkSize]);
            
    for (int i = 0; i < numSamples; i++) {
        for (int channel = 0; channel < nChannels; channel++) {
            int sampleIndex = samplesStartIndex + (numBytesPerBlock * i) + channel * numBytesPerSample;

            if (bitDepth == 8) {
                T sample = singleByteToSample(fileData[sampleIndex]);
                samples[channel].push_back(sample);
               // int16_t &dd =  (int16_t &)(fileData[sampleIndex]);
              //  uint8_t tmp  = fileData[sampleIndex];
              //   int16_t *tmp2 = (int16_t *)&tmp;
               //   int16_t tmp3 = *tmp2;
                int16_t sample16 = (int16_t) (fileData[sampleIndex] - 0x80) << 8;

                 
                rawPCMint16[channel].push_back(sample16);
            } else if (bitDepth == 16) {
                int16_t sampleAsInt = twoBytesToInt(fileData, sampleIndex);
                T sample = sixteenBitIntToSample(sampleAsInt);
                samples[channel].push_back(sample);
                rawPCMint16[channel].push_back(sampleAsInt);
                
            } else if (bitDepth == 24) {
                int32_t sampleAsInt = 0;
                sampleAsInt = (fileData[sampleIndex + 2] << 16) | (fileData[sampleIndex + 1] << 8) | fileData[sampleIndex];

                if (sampleAsInt & 0x800000) //  if the 24th bit is set, this is a negative number in 24-bit world
                    sampleAsInt = sampleAsInt | ~0xFFFFFF; // so make sure sign is extended to the 32 bit float

                T sample = (T) sampleAsInt / (T) 8388608.;
                samples[channel].push_back(sample);
            }else if (bitDepth == 32 &&  audioFormat == 3) {
               float sampleAsFloat = 0;
                int tmp =  ( (fileData[sampleIndex + 3] << 24) |(fileData[sampleIndex + 2] << 16) | (fileData[sampleIndex + 1] << 8) | fileData[sampleIndex]);
                float *tmp2 = (float *)&tmp;
                sampleAsFloat = *tmp2;
                if( sampleAsFloat > 1)
                    sampleAsFloat = 1;
                if(sampleAsFloat < -1)
                    sampleAsFloat = -1;
                rawPCMFloat.push_back(sampleAsFloat);
                samples[channel].push_back(sampleAsFloat);
                const float  m_max = INT_MAX;
                const float  m_min = -(float) INT_MIN;


                int v;
                if (sampleAsFloat >= 1) {
                    v = INT_MAX;
                } else if (sampleAsFloat <= -1) {
                    v = INT_MIN;
                } else if (sampleAsFloat >= 0) {
                    v = (int) (sampleAsFloat * m_max + 0.5f);
                } else {
                    v = (int) (sampleAsFloat * m_min - 0.5f);
                }


                int16_t sample16 = v >> (8 * sizeof (int) - 16);
                rawPCMint16[channel].push_back(sample16);
                ///////////////////////////////////////
            } else {
                assert(false);
            }
        }
    }

    captionlist caption_list = NULL;

    caption_list = push_back_caption(caption_list, "Sine", 0, 0x0000FF);

    coordlist coordinate_list = NULL;
    
    //fill in with range
   // std::vector<int> v(numSamples) ; // vector with 100 ints.
   // std::iota (std::begin(v), std::end(v), 0); // Fill with 0, 1, ..., 99

   // coordinate_list = push_back_coords(coordinate_list, 0, &samples[1][0] , 1024);
    //populate plot parameter object
 //   params1 = new plot_params("Time (s)", "Speed (Mbit/s)", caption_list, coordinate_list, 800, 800,{1, 1}, {10, 10 }, {0, 0});
    //  params->scale.x = .25;
    //   params->scale.y = .2;
    //  Plot_Window_params win_param;
   // params1->push_back(0, 1, 1);
  //  params1->push_back(0, 5, 6);
   // params1->push_back(0, 9, 9);
    
   params1 = new plot_params( "x", "y", caption_list, coordinate_list, 800, 400, {1024, 1},{0, -1 });
    
   push_back_plot_win(params1);

    return true;
}

//=============================================================

template <class T>
bool AudioFile<T>::decodeAiffFile(std::vector<uint8_t>& fileData) {
    // -----------------------------------------------------------
    // HEADER CHUNK
    std::string headerChunkID(fileData.begin(), fileData.begin() + 4);
    //int32_t fileSizeInBytes = fourBytesToInt (fileData, 4, Endianness::BigEndian) + 8;
    std::string format(fileData.begin() + 8, fileData.begin() + 12);

    // -----------------------------------------------------------
    // try and find the start points of key chunks
    int indexOfCommChunk = getIndexOfString(fileData, "COMM");
    int indexOfSoundDataChunk = getIndexOfString(fileData, "SSND");

    // if we can't find the data or format chunks, or the IDs/formats don't seem to be as expected
    // then it is unlikely we'll able to read this file, so abort
    if (indexOfSoundDataChunk == -1 || indexOfCommChunk == -1 || headerChunkID != "FORM" || format != "AIFF") {
        std::cout << "ERROR: this doesn't seem to be a valid AIFF file" << std::endl;
        return false;
    }

    // -----------------------------------------------------------
    // COMM CHUNK
    int p = indexOfCommChunk;
    std::string commChunkID(fileData.begin() + p, fileData.begin() + p + 4);
    //int32_t commChunkSize = fourBytesToInt (fileData, p + 4, Endianness::BigEndian);
    int16_t numChannels = twoBytesToInt(fileData, p + 8, Endianness::BigEndian);
    int32_t numSamplesPerChannel = fourBytesToInt(fileData, p + 10, Endianness::BigEndian);
    bitDepth = (int) twoBytesToInt(fileData, p + 14, Endianness::BigEndian);
    sampleRate = getAiffSampleRate(fileData, p + 16);

    // check the sample rate was properly decoded
    if (sampleRate == -1) {
        std::cout << "ERROR: this AIFF file has an unsupported sample rate" << std::endl;
        return false;
    }

    // check the number of channels is mono or stereo
    if (numChannels < 1 || numChannels > 2) {
        std::cout << "ERROR: this AIFF file seems to be neither mono nor stereo (perhaps multi-track, or corrupted?)" << std::endl;
        return false;
    }

    // check bit depth is either 8, 16 or 24 bit
    if (bitDepth != 8 && bitDepth != 16 && bitDepth != 24 && bitDepth != 32) {
        std::cout << "ERROR: this file has a bit depth that is not 8, 16, 24 or 32 bits" << std::endl;
        return false;
    }

    // -----------------------------------------------------------
    // SSND CHUNK
    int s = indexOfSoundDataChunk;
    std::string soundDataChunkID(fileData.begin() + s, fileData.begin() + s + 4);
    int32_t soundDataChunkSize = fourBytesToInt(fileData, s + 4, Endianness::BigEndian);
    int32_t offset = fourBytesToInt(fileData, s + 8, Endianness::BigEndian);
    //int32_t blockSize = fourBytesToInt (fileData, s + 12, Endianness::BigEndian);

    int numBytesPerSample = bitDepth / 8;
    int numBytesPerFrame = numBytesPerSample * numChannels;
    int totalNumAudioSampleBytes = numSamplesPerChannel * numBytesPerFrame;
    int samplesStartIndex = s + 16 + (int) offset;

    // sanity check the data
    if ((soundDataChunkSize - 8) != totalNumAudioSampleBytes || totalNumAudioSampleBytes > (fileData.size() - samplesStartIndex)) {
        std::cout << "ERROR: the metadatafor this file doesn't seem right" << std::endl;
        return false;
    }

    clear();
    samples.resize(numChannels);

    for (int i = 0; i < numSamplesPerChannel; i++) {
        for (int channel = 0; channel < numChannels; channel++) {
            int sampleIndex = samplesStartIndex + (numBytesPerFrame * i) + channel * numBytesPerSample;

            if (bitDepth == 8) {
                int8_t sampleAsSigned8Bit = (int8_t) fileData[sampleIndex];
                T sample = (T) sampleAsSigned8Bit / (T) 128.;
                samples[channel].push_back(sample);
            } else if (bitDepth == 16) {
                int16_t sampleAsInt = twoBytesToInt(fileData, sampleIndex, Endianness::BigEndian);
                T sample = sixteenBitIntToSample(sampleAsInt);
                samples[channel].push_back(sample);
            } else if (bitDepth == 24) {
                int32_t sampleAsInt = 0;
                sampleAsInt = (fileData[sampleIndex] << 16) | (fileData[sampleIndex + 1] << 8) | fileData[sampleIndex + 2];

                if (sampleAsInt & 0x800000) //  if the 24th bit is set, this is a negative number in 24-bit world
                    sampleAsInt = sampleAsInt | ~0xFFFFFF; // so make sure sign is extended to the 32 bit float

                T sample = (T) sampleAsInt / (T) 8388608.;
                samples[channel].push_back(sample);
            } else {
                assert(false);
            }
        }
    }

    return true;
}

const char *SdlAudioFormatToString(int sdlAudioType) {
  switch(sdlAudioType) {
  case AUDIO_U8: return "AUDIO_U8";
  case AUDIO_S8: return "AUDIO_S8";
  case AUDIO_U16LSB: return "AUDIO_U16LSB";
  case AUDIO_U16MSB: return "AUDIO_U16MSB";
  case AUDIO_S16LSB: return "AUDIO_S16LSB";
  case AUDIO_S16MSB: return "AUDIO_S16MSB";
  case AUDIO_F32: return "AUDIO_F32";
  case AUDIO_S32LSB: return "AUDIO_S32LSB";
  case AUDIO_S32MSB: return "AUDIO_S32MSB";
  
  //case AUDIO_F32LSB: return "AUDIO_F32LSB";
  case AUDIO_F32MSB: return "AUDIO_F32MSB";
  
  default: return "(unknown)";
  }
}


template <class T>
void AudioFile<T>::clear() {
    


    for (auto& kv : rawPCMint16) {
          kv.clear();
    }

    rawPCMint16.clear();

    for (int i = 0; i < samples.size(); i++) {
        samples[i].clear();
    }

    samples.clear();
    
    rawPCMFloat.clear();
            
}

template <class T>
bool AudioFile<T>::stop() {
    
   m_Soppped = true;
   SDL_PauseAudio(1);
   SDL_CloseAudio();
   


    
    clear();
   
            
    return true;
}

template <class T>
bool AudioFile<T>::play() {
   
   /*
       //https://gist.github.com/vl-80/511943db52459890e30501aa0885a793
    //https://skia.googlesource.com/third_party/sdl/+/refs/heads/master/test/loopwavequeue.c
    	SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8 *wavBuffer;

	SDL_LoadWAV("/root/Desktop/delete/test.wav", &wavSpec, &wavBuffer, &wavLength);
	
	// open audio device
        
        wavSpec.freq  = sampleRate;                   
        wavSpec.format =  bitDepth == 16? AUDIO_S16LSB: AUDIO_S8;     
        wavSpec.channels = nChannels;            
        wavSpec.silence;            
        wavSpec.samples = 0;            
        wavSpec.padding;            
        wavSpec.size;               
  

	SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

        SDL_AudioFormat fmt = wavSpec.format;
        
        
   

        if (SDL_AUDIO_ISFLOAT(fmt)) {
            printf("floating point data\n");
        } else {
            printf("integer data\n");
        }
        printf("%d bits per sample\n", (int) SDL_AUDIO_BITSIZE(fmt));


	// play audio

	//int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
        int success = SDL_QueueAudio(deviceId, &rawPCM[0], rawPCM.size());
	SDL_PauseAudioDevice(deviceId, 0);

	// keep window open enough to hear the sound

	//SDL_Delay(3000);
        
        sleep( 1000);
	// clean up

	SDL_CloseAudioDevice(deviceId);
	//SDL_FreeWAV(wavBuffer);
        
    */  
    
    
    SDL_AudioSpec desiredSpec;

    desiredSpec.freq = sampleRate;
    
    
    desiredSpec.channels = nChannels;
    desiredSpec.samples = 1024;  // single channel. Lentght = samplees*nochanges*bitdepth
    if(  audioFormat == 1 && (   bitDepth == 16  || bitDepth == 8  ))
    {
        desiredSpec.format = bitDepth == 16 ? AUDIO_S16LSB: AUDIO_S8;
        desiredSpec.callback = audio_callback_INT;
    }
    else if( audioFormat == 3  && bitDepth == 32  )
    {
        
        desiredSpec.format = AUDIO_F32;
        desiredSpec.callback = audio_callback_FLOAT;
     }
    else
    {
        std::cout << "Unsupported format " << "audioFormat" ;
    }
    
  
    
    desiredSpec.userdata = this;

    SDL_AudioSpec obtainedSpec;

    // you might want to look for errors here
    SDL_OpenAudio(&desiredSpec, &obtainedSpec);

    
    if( (desiredSpec.format != obtainedSpec.format)  ||  (desiredSpec.samples  != obtainedSpec.samples  ) )
    {
        if((desiredSpec.format != obtainedSpec.format) )
        {
            if ( obtainedSpec.format == AUDIO_S16LSB ||  obtainedSpec.format == AUDIO_S8 ) {
                 desiredSpec.callback = audio_callback_INT;


            } else if ( obtainedSpec.format == AUDIO_F32) {


                int len = rawPCMInt.size();
                rawPCMFloat.clear();
                for (int sampleIndex = 0; sampleIndex < len/2; sampleIndex = sampleIndex+2) 
                {

                int16_t sampleAsInt =  (rawPCMInt[sampleIndex + 1] << 8) | rawPCMInt[sampleIndex];

                 float const u = sampleAsInt/32768.0;

                  rawPCMFloat.push_back(u);
                }
                  desiredSpec.callback = audio_callback_FLOAT;
                  desiredSpec.format = AUDIO_F32;
            } 
            else {
                std::cout << "Unsupported format " << "audioFormat";
                exit(0);
            }
        }

        if (desiredSpec.samples != obtainedSpec.samples) {

            desiredSpec.samples = desiredSpec.samples * desiredSpec.samples / obtainedSpec.samples;
        }
        SDL_PauseAudio(1);
        SDL_CloseAudio();
        SDL_OpenAudio(&desiredSpec, &obtainedSpec);
        
    }// end if desired format != desired spec
    
    process.Init(obtainedSpec.freq);
    printf("obtainedSpec   %d  freq with audio format %s, %d channels, and %d samples buffer.\n",
       (int)obtainedSpec.freq, SdlAudioFormatToString(obtainedSpec.format), obtainedSpec.channels,  obtainedSpec.samples);
   
      
    // start play audio
    SDL_PauseAudio(0);
    
   /*  int duration = 1000;
    double Hz = 440;
     BeepObject bo;
    bo.freq = Hz;
    bo.samplesLeft = duration * FREQUENCY / 1000;
    
    SDL_LockAudio();
    beeps.push(bo);
    SDL_UnlockAudio();
    */
    return true;
}

//=============================================================

template <class T>
uint32_t AudioFile<T>::getAiffSampleRate(std::vector<uint8_t>& fileData, int sampleRateStartIndex) {
    for (auto it : aiffSampleRateTable) {
        if (tenByteMatch(fileData, sampleRateStartIndex, it.second, 0))
            return it.first;
    }

    return -1;
}

//=============================================================

template <class T>
bool AudioFile<T>::tenByteMatch(std::vector<uint8_t>& v1, int startIndex1, std::vector<uint8_t>& v2, int startIndex2) {
    for (int i = 0; i < 10; i++) {
        if (v1[startIndex1 + i] != v2[startIndex2 + i])
            return false;
    }

    return true;
}

//=============================================================

template <class T>
void AudioFile<T>::addSampleRateToAiffData(std::vector<uint8_t>& fileData, uint32_t sampleRate) {
    if (aiffSampleRateTable.count(sampleRate) > 0) {
        for (int i = 0; i < 10; i++)
            fileData.push_back(aiffSampleRateTable[sampleRate][i]);
    }
}

//=============================================================

template <class T>
bool AudioFile<T>::save(std::string filePath, AudioFileFormat format) {
    if (format == AudioFileFormat::Wave) {
        return saveToWaveFile(filePath);
    } else if (format == AudioFileFormat::Aiff) {
        return saveToAiffFile(filePath);
    }

    return false;
}

//=============================================================

template <class T>
bool AudioFile<T>::saveToWaveFile(std::string filePath) {
    std::vector<uint8_t> fileData;

    int32_t dataChunkSize = getNumSamplesPerChannel() * (getNumChannels() * bitDepth / 8);

    // -----------------------------------------------------------
    // HEADER CHUNK
    addStringToFileData(fileData, "RIFF");

    // The file size in bytes is the header chunk size (4, not counting RIFF and WAVE) + the format
    // chunk size (24) + the metadata part of the data chunk plus the actual data chunk size
    int32_t fileSizeInBytes = 4 + 24 + 8 + dataChunkSize;
    addInt32ToFileData(fileData, fileSizeInBytes);

    addStringToFileData(fileData, "WAVE");

    // -----------------------------------------------------------
    // FORMAT CHUNK
    addStringToFileData(fileData, "fmt ");
    addInt32ToFileData(fileData, 16); // format chunk size (16 for PCM)
    addInt16ToFileData(fileData, 1); // audio format = 1
    addInt16ToFileData(fileData, (int16_t) getNumChannels()); // num channels
    addInt32ToFileData(fileData, (int32_t) sampleRate); // sample rate

    int32_t numBytesPerSecond = (int32_t) ((getNumChannels() * sampleRate * bitDepth) / 8);
    addInt32ToFileData(fileData, numBytesPerSecond);

    int16_t numBytesPerBlock = getNumChannels() * (bitDepth / 8);
    addInt16ToFileData(fileData, numBytesPerBlock);

    addInt16ToFileData(fileData, (int16_t) bitDepth);

    // -----------------------------------------------------------
    // DATA CHUNK
    addStringToFileData(fileData, "data");
    addInt32ToFileData(fileData, dataChunkSize);

    for (int i = 0; i < getNumSamplesPerChannel(); i++) {
        for (int channel = 0; channel < getNumChannels(); channel++) {
            if (bitDepth == 8) {
                uint8_t byte = sampleToSingleByte(samples[channel][i]);
                fileData.push_back(byte);
            } else if (bitDepth == 16) {
                int16_t sampleAsInt = sampleToSixteenBitInt(samples[channel][i]);
                addInt16ToFileData(fileData, sampleAsInt);
            } else if (bitDepth == 24) {
                int32_t sampleAsIntAgain = (int32_t) (samples[channel][i] * (T) 8388608.);

                uint8_t bytes[3];
                bytes[2] = (uint8_t) (sampleAsIntAgain >> 16) & 0xFF;
                bytes[1] = (uint8_t) (sampleAsIntAgain >> 8) & 0xFF;
                bytes[0] = (uint8_t) sampleAsIntAgain & 0xFF;

                fileData.push_back(bytes[0]);
                fileData.push_back(bytes[1]);
                fileData.push_back(bytes[2]);
            } else {
                assert(false && "Trying to write a file with unsupported bit depth");
                return false;
            }
        }
    }

    // check that the various sizes we put in the metadata are correct
    if (fileSizeInBytes != (fileData.size() - 8) || dataChunkSize != (getNumSamplesPerChannel() * getNumChannels() * (bitDepth / 8))) {
        std::cout << "ERROR: couldn't save file to " << filePath << std::endl;
        return false;
    }

    // try to write the file
    return writeDataToFile(fileData, filePath);
}

//=============================================================

template <class T>
bool AudioFile<T>::saveToAiffFile(std::string filePath) {
    std::vector<uint8_t> fileData;

    int32_t numBytesPerSample = bitDepth / 8;
    int32_t numBytesPerFrame = numBytesPerSample * getNumChannels();
    int32_t totalNumAudioSampleBytes = getNumSamplesPerChannel() * numBytesPerFrame;
    int32_t soundDataChunkSize = totalNumAudioSampleBytes + 8;

    // -----------------------------------------------------------
    // HEADER CHUNK
    addStringToFileData(fileData, "FORM");

    // The file size in bytes is the header chunk size (4, not counting FORM and AIFF) + the COMM
    // chunk size (26) + the metadata part of the SSND chunk plus the actual data chunk size
    int32_t fileSizeInBytes = 4 + 26 + 16 + totalNumAudioSampleBytes;
    addInt32ToFileData(fileData, fileSizeInBytes, Endianness::BigEndian);

    addStringToFileData(fileData, "AIFF");

    // -----------------------------------------------------------
    // COMM CHUNK
    addStringToFileData(fileData, "COMM");
    addInt32ToFileData(fileData, 18, Endianness::BigEndian); // commChunkSize
    addInt16ToFileData(fileData, getNumChannels(), Endianness::BigEndian); // num channels
    addInt32ToFileData(fileData, getNumSamplesPerChannel(), Endianness::BigEndian); // num samples per channel
    addInt16ToFileData(fileData, bitDepth, Endianness::BigEndian); // bit depth
    addSampleRateToAiffData(fileData, sampleRate);

    // -----------------------------------------------------------
    // SSND CHUNK
    addStringToFileData(fileData, "SSND");
    addInt32ToFileData(fileData, soundDataChunkSize, Endianness::BigEndian);
    addInt32ToFileData(fileData, 0, Endianness::BigEndian); // offset
    addInt32ToFileData(fileData, 0, Endianness::BigEndian); // block size

    for (int i = 0; i < getNumSamplesPerChannel(); i++) {
        for (int channel = 0; channel < getNumChannels(); channel++) {
            if (bitDepth == 8) {
                uint8_t byte = sampleToSingleByte(samples[channel][i]);
                fileData.push_back(byte);
            } else if (bitDepth == 16) {
                int16_t sampleAsInt = sampleToSixteenBitInt(samples[channel][i]);
                addInt16ToFileData(fileData, sampleAsInt, Endianness::BigEndian);
            } else if (bitDepth == 24) {
                int32_t sampleAsIntAgain = (int32_t) (samples[channel][i] * (T) 8388608.);

                uint8_t bytes[3];
                bytes[0] = (uint8_t) (sampleAsIntAgain >> 16) & 0xFF;
                bytes[1] = (uint8_t) (sampleAsIntAgain >> 8) & 0xFF;
                bytes[2] = (uint8_t) sampleAsIntAgain & 0xFF;

                fileData.push_back(bytes[0]);
                fileData.push_back(bytes[1]);
                fileData.push_back(bytes[2]);
            } else {
                assert(false && "Trying to write a file with unsupported bit depth");
                return false;
            }
        }
    }

    // check that the various sizes we put in the metadata are correct
    if (fileSizeInBytes != (fileData.size() - 8) || soundDataChunkSize != getNumSamplesPerChannel() * numBytesPerFrame + 8) {
        std::cout << "ERROR: couldn't save file to " << filePath << std::endl;
        return false;
    }

    // try to write the file
    return writeDataToFile(fileData, filePath);
}

//=============================================================

template <class T>
bool AudioFile<T>::writeDataToFile(std::vector<uint8_t>& fileData, std::string filePath) {
    std::ofstream outputFile(filePath, std::ios::binary);

    if (outputFile.is_open()) {
        for (int i = 0; i < fileData.size(); i++) {
            char value = (char) fileData[i];
            outputFile.write(&value, sizeof (char));
        }

        outputFile.close();

        return true;
    }

    return false;
}

//=============================================================

template <class T>
void AudioFile<T>::addStringToFileData(std::vector<uint8_t>& fileData, std::string s) {
    for (int i = 0; i < s.length(); i++)
        fileData.push_back((uint8_t) s[i]);
}

//=============================================================

template <class T>
void AudioFile<T>::addInt32ToFileData(std::vector<uint8_t>& fileData, int32_t i, Endianness endianness) {
    uint8_t bytes[4];

    if (endianness == Endianness::LittleEndian) {
        bytes[3] = (i >> 24) & 0xFF;
        bytes[2] = (i >> 16) & 0xFF;
        bytes[1] = (i >> 8) & 0xFF;
        bytes[0] = i & 0xFF;
    } else {
        bytes[0] = (i >> 24) & 0xFF;
        bytes[1] = (i >> 16) & 0xFF;
        bytes[2] = (i >> 8) & 0xFF;
        bytes[3] = i & 0xFF;
    }

    for (int i = 0; i < 4; i++)
        fileData.push_back(bytes[i]);
}

//=============================================================

template <class T>
void AudioFile<T>::addInt16ToFileData(std::vector<uint8_t>& fileData, int16_t i, Endianness endianness) {
    uint8_t bytes[2];

    if (endianness == Endianness::LittleEndian) {
        bytes[1] = (i >> 8) & 0xFF;
        bytes[0] = i & 0xFF;
    } else {
        bytes[0] = (i >> 8) & 0xFF;
        bytes[1] = i & 0xFF;
    }

    fileData.push_back(bytes[0]);
    fileData.push_back(bytes[1]);
}



template <class T>
AudioFileFormat AudioFile<T>::determineAudioFileFormat(std::vector<uint8_t>& fileData) {
    std::string header(fileData.begin(), fileData.begin() + 4);

    if (header == "RIFF")
        return AudioFileFormat::Wave;
    else if (header == "FORM")
        return AudioFileFormat::Aiff;
    else
        return AudioFileFormat::Error;
}

//=============================================================

template <class T>
int32_t AudioFile<T>::fourBytesToInt(std::vector<uint8_t>& source, int startIndex, Endianness endianness) {
    int32_t result;

    if (endianness == Endianness::LittleEndian)
        result = (source[startIndex + 3] << 24) | (source[startIndex + 2] << 16) | (source[startIndex + 1] << 8) | source[startIndex];
    else
        result = (source[startIndex] << 24) | (source[startIndex + 1] << 16) | (source[startIndex + 2] << 8) | source[startIndex + 3];

    return result;
}

//=============================================================

template <class T>
int16_t AudioFile<T>::twoBytesToInt(std::vector<uint8_t>& source, int startIndex, Endianness endianness) {
    int16_t result;

    if (endianness == Endianness::LittleEndian)
        result = (source[startIndex + 1] << 8) | source[startIndex];
    else
        result = (source[startIndex] << 8) | source[startIndex + 1];

    return result;
}

//=============================================================

template <class T>
int AudioFile<T>::getIndexOfString(std::vector<uint8_t>& source, std::string stringToSearchFor) {
    int index = -1;
    int stringLength = (int) stringToSearchFor.length();

    for (int i = 0; i < source.size() - stringLength; i++) {
        std::string section(source.begin() + i, source.begin() + i + stringLength);

        if (section == stringToSearchFor) {
            index = i;
            break;
        }
    }

    return index;
}

//=============================================================

template <class T>
T AudioFile<T>::sixteenBitIntToSample(int16_t sample) {
    return static_cast<T> (sample) / static_cast<T> (32768.);
}

//=============================================================

template <class T>
int16_t AudioFile<T>::sampleToSixteenBitInt(T sample) {
    sample = clamp(sample, -1., 1.);
    return static_cast<int16_t> (sample * 32767.);
}

//=============================================================

template <class T>
uint8_t AudioFile<T>::sampleToSingleByte(T sample) {
    sample = clamp(sample, -1., 1.);
    sample = (sample + 1.) / 2.;
    return static_cast<uint8_t> (sample * 255.);
}

//=============================================================

template <class T>
T AudioFile<T>::singleByteToSample(uint8_t sample) {
       return static_cast<T> (sample - 128) / static_cast<T> (128.);
}

//=============================================================

template <class T>
T AudioFile<T>::clamp(T value, T minValue, T maxValue) {
    value = std::min(value, maxValue);
    value = std::max(value, minValue);
    return value;
}


 // single channel. Lentght = samplees*nochanges*bitdepth
template <class T>
void AudioFile<T>::generateSamplesInt(Uint8 *stream, int length)
{
   
    if( alradyPlayed + length <  rawPCMInt.size()  )
    {   analyzeWave();
        SDL_memcpy(stream,  &rawPCMInt[alradyPlayed], length); 
        alradyPlayed = alradyPlayed + length;
    }else
    {
        stop();

    }
    
}
 // single channel. Lentght = samplees*nochanges*bitdepth
template <class T>
void AudioFile<T>::generateSamplesFloat(Uint8 *stream, int length)
{
    
    if( alradyPlayed + length/4 <  rawPCMFloat.size()  )
    {
        analyzeWave();
        SDL_memcpy(stream,  &rawPCMFloat[alradyPlayed], length); 
        alradyPlayed = alradyPlayed + length/4;
    }else
    {
        SDL_PauseAudio(1);
        SDL_CloseAudio();
    }
         
    
}


 // single channel. Lentght = samplees*nochanges*bitdepth
template <class T>
void AudioFile<T>:: audio_callback_INT(void *data, Uint8 * stream, int length)
{
    printf("audio_callback_INT %d\n",length  );
     AudioFile *sound = reinterpret_cast<AudioFile*>(data);
     
      sound->generateSamplesInt(stream, length);
}


 // single channel. Lentght = samplees*nochanges*bitdepth
template <class T>
void AudioFile<T>:: audio_callback_FLOAT(void *data, Uint8 * stream, int length)
{
  printf("audio_callback_float %d\n",length  );
    AudioFile *sound = reinterpret_cast<AudioFile*>(data);
    sound->generateSamplesFloat(stream, length);
}


//===========================================================
template class AudioFile<float>;
template class AudioFile<double>;
template class AudioFile<short int>;