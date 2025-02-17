#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <openssl/md5.h>
#include <unistd.h>  // For execvp

std::string computeMD5(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    MD5_CTX md5Context;
    MD5_Init(&md5Context);

    char buffer[4096];
    while (file.read(buffer, sizeof(buffer)) || file.gcount()) {
        MD5_Update(&md5Context, buffer, file.gcount());
    }

    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5_Final(hash, &md5Context);

    std::ostringstream result;
    for (unsigned char c : hash) {
        result << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    }
    return result.str();
}

int main() {
    std::string filename="game";
    std::string hashvalue="4b4b1730ec2174e32ac75270e332159c";

    std::string Result=computeMD5(filename);
    std::cout<<Result<<std::endl;
    if(hashvalue==Result){
        std::cout<<"Valid File"<<std::endl;
        // Prepare arguments
        char* args[] = { (char*)"./game", nullptr };
        // Execute the file
        execvp(args[0], args);

        // If execvp fails
        perror("execvp failed");
        return 1;
    }
    else{
        std::cout<<"Invalid File"<<std::endl;
    }
    return 0;
}
