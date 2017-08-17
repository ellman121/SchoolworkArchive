/*************************************************************************//**
 * @file functions.h 
 *
 * @brief this file contains the basic information for functions.cpp.  It 
 * should be included with function.cpp.
 ****************************************************************************/

#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <io.h>
#include <direct.h>

using namespace std;

/*******************************************************************************
 *             Constant Variables, defines, structures and Enums
 ******************************************************************************/

/*!
 * @brief Holds a mp3 file tag
 *
 * @details Each field is a number of bytes in the tag
 */
struct mp3Tag
{
    char TAG[3];
    char songTitle[30];
    char artistName[30];
    char albumName[30];
    char year[4];
    char comment[28];
    char NULLBYTE;
    unsigned char track;
    unsigned char genre;
};

/*!
 * @brief Global constant string array for the genrelist.
 */
const string genrelist[150] = {"000. Blues", "001. Classic Rock", "002. Country",\
		"003. Dance", "004. Disco", "005. Funk", "006. Grunge", "007. Hip-Hop",\
		"008. Jazz", "009. Metal", "010. New Age", "011. Oldies", "012. Other",\
		"013. Pop", "014. R&B", "015. Reggae", "016. Rock", "017. Techno", \
		"018. Industtrial", "019. Alternative", "020. Ska", "021. Death Metal" \
		"022. Pranks", "023. Soundtrack", "024. Euro-Techno", "025. Ambient", \
		"026. Trip-Hop", "027. Vocal", "028. Jazz+Funk", "029. Fusion", \
		"031. Trance", "032. Classical", "033. Instrumental", "034. Acid", \
		"035. House", "036. Game", "037. Sound Clip", "038. Gospel", \
		"039. Noise", "040. Alternative Rock", "041. Bass", "042. Soul", \
		"043. Punk", "044. Space", "045. Meditative", "046. Instrumental Pop", \
		"047. Instrumental Rock", "048. Ethnic", "049. Gothic", \
		"050. Darkwave", "051. Techno-Industrial", "052. Electronic", \
		"053. Pop-Folk", "054. Eurodance", "055. Dream", "056. Southern Rock", \
		"057. Comedy", "058. Cult", "059. Gangsta", "060. Top 40", \
		"061. Christian Rap", "062. Pop/Funk", "063. Jungle", "064. Native US",\
		"065. Cabaret", "066. New Wave", "067. Psychadelic", "068. Rave", \
		"069. Showtunes", "070. Trailer", "071. Lo-Fi", "072. Tribal", \
		"073. Acid Punk", "074. Acid Jazz", "075. Polka", \
		"076. Retro", "077. Musical", "078. Rock & Roll", "079, Hard Rock", \
		"080. Folk", "081. Folk-Rock", "082. National Folk", "083. Swing", \
		"084. Fast Fusion", "085. Bebob", "086. Latin", "087. Revival", \
		"088. Celtic", "089. Bluegrass", "090. Avantgarde","091. Gothic Rock", \
		"092. Progressive Rock", "093. Psychedelic Rock", \
		"094. Symphonic Rock", "095. Slow Rock", "096. Big Band", \
		"097. Chorus", "098. Easy Listening", "099. Acoustic", "100. Humour", \
		"101. Speach", "102. Chanson", "103. Opera", "104. Chamber Music", \
		"105. Sonata", "106. Symphony", "107, Booty Bass", "108, Primus", \
		"109. Porn Groove", "110, Satire", "111. Slow Jam", "112. Club", \
		"113. Tango", "114. Samba", "115. Folklore", "116. Ballad", \
		"117. Power Balla", "118. Rhythmic Soul","119. Freestyle","120. Duet",\
		"121. Punk Rock", "122. Drum Solo", "123. Acapella", "124. Euro-House",\
		"125. Dance Hall", "126. Goa", "127. Drum & Bass", "128. Club-Hosue", \
		"129. Hardcore", "130. Terror", "131. Indie", "132. BritPop", \
		"133. Negerpunk", "134. Polsk Punk", "135. Beat", \
		"136. Christian Gangsta Rap", "137. Heavy Metal", \
		"138. Black Metal", "139. Crossover", "140. Contemporary Christian", \
		"141. Christian Rock", "142. Merengue", "143. Salsa", \
		"144. Trash Metal", "145. Anime", "146. Jpop", "147. Synthpop"};


/*******************************************************************************
 *                         Function Prototypes
 ******************************************************************************/
void changeFile(string arguments[15], mp3Tag newTag);
void exitWithError();
int  extractIntFromString(string str);
void formatArgumentArray(string oldArray[20], string formattedArray[15], 
												int numberOfIterations);
void modifyTag(string arguments[15], mp3Tag &destTag);
void organizeArray(int numberOfIterations, string oldArray[20],
							string formattedArray[20], bool flagArray[8]);
void printGenrelist();
void printTag(mp3Tag &tag);
void printUsageStatement();
void readFile(ifstream &fin, string fileName, mp3Tag &destTag);
void writeFile(ofstream &fout, string filename, mp3Tag &writeTag);

#endif