// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define WIN32_NO_STATUS
#include <Windows.h>
#undef WIN32_NO_STATUS
#include <iostream>
#include <ncryptprotect.h>
#include "base64.hpp"

void getNCryptDescriptorHandle(NCRYPT_DESCRIPTOR_HANDLE& hProvider);
void encryptData(const NCRYPT_DESCRIPTOR_HANDLE& hProvider, const BYTE* plaintextData, const ULONG plaintextDataSize, BYTE*& ciphertext_ptr, ULONG& ciphertextLength);
void decryptData(const BYTE* ciphertextData, const ULONG ciphertextLength, BYTE*& plaintext_ptr, ULONG& plaintextLength);
BYTE* nullTerminateString(BYTE* charArray, ULONG arrayLength);

int main()
{

	NCRYPT_DESCRIPTOR_HANDLE hProvider;
	BYTE plaintextData[] = "Hello, World!";
	ULONG plaintextDataSize = sizeof(plaintextData) - 1;
	BYTE* ciphertext_ptr = nullptr;
	ULONG ciphertextLength = 0;

	getNCryptDescriptorHandle(hProvider);
	encryptData(hProvider, plaintextData, plaintextDataSize, ciphertext_ptr, ciphertextLength);

	std::string base64ciphertext = base64_encode(ciphertext_ptr, static_cast<int>(ciphertextLength));
	//std::string base64ciphertext = "MIIB4wYJKoZIhvcNAQcDoIIB1DCCAdACAQIxggF9ooIBeQIBBDCCATsEggEGAQAAANCMnd8BFdERjHoAwE/Cl+sBAAAAfrFS7llKI0y/QmyLln5IyQQAAAACAAAAAAAQZgAAAAEAACAAAADLPGugqdXY0E/uEb7O6P6BnaLz4GsZlrloKwrJKycyxgAAAAAOgAAAAAIAACAAAABOJGmCFG4EpP7PeN5g2lHN4j9f0mpwGw0KQvqT7aQ5QjAAAAB8eH3HbN3AWGYzRwWMeSsBIzkkJIr2H87wYKi3vQC9y/LbmajaoTHBiGhgjj5iLU9AAAAAb+ODppLBgs2G28r0/9cTleK5XPsSwGkCZ4/zg5A/l6mEuzTPsGyJKmyHha/9CBfHIqgLf+uNl7/JuCjD7jtRuTAvBgkrBgEEAYI3SgEwIgYKKwYBBAGCN0oBCDAUMBIwEAwFTE9DQUwMB21hY2hpbmUwCwYJYIZIAWUDBAEtBCh8g5XobqWfZD1au0RvwsAkptGv0q6FLlypiVtld7OHD+qNDI5NfrE+MEoGCSqGSIb3DQEHATAeBglghkgBZQMEAS4wEQQMducHiN7XgoWG95vkAgEQgB2V4QJiFshJvOoQKo3Cj5qBitRm/+dDzjdPFW9E3w==";
	std::cout << base64ciphertext << std::endl;

	std::vector<BYTE> cipherTextDecoded = base64_decode(base64ciphertext);
	BYTE* decodedCiphertext_ptr = cipherTextDecoded.data();
	ciphertextLength = static_cast<ULONG>(cipherTextDecoded.size());


	BYTE* decodedDecryptedText;
	ULONG decodedDecryptedTextLength;

	decryptData(decodedCiphertext_ptr, ciphertextLength, decodedDecryptedText, decodedDecryptedTextLength);
	decodedDecryptedText = nullTerminateString(decodedDecryptedText, decodedDecryptedTextLength);

	std::cout << "Decrypted Text: " << decodedDecryptedText << std::endl;

}

BYTE* nullTerminateString(BYTE* charArray, ULONG arrayLength) {
	BYTE* destination = new BYTE[arrayLength + 1];
#pragma warning (disable : 4996)
	strncpy((char*)destination, (char*)charArray, arrayLength);
	destination[arrayLength] = 0;
	return destination;
}

void getNCryptDescriptorHandle(NCRYPT_DESCRIPTOR_HANDLE &hProvider) {
	DWORD flags = 0;
	LPCWSTR providerDescriptorString = L"LOCAL=machine";

	SECURITY_STATUS status = NCryptCreateProtectionDescriptor(providerDescriptorString, flags, &hProvider);
	if (status != ERROR_SUCCESS) {
		std::cout << "NCryptCreateProtectionDescriptor failed with error code: " << status << std::endl;
		exit(1);
	}
}

void encryptData(const NCRYPT_DESCRIPTOR_HANDLE &hProvider, const BYTE* plaintextData, const ULONG plaintextDataSize, BYTE* &ciphertext_ptr, ULONG &ciphertextLength) {
	NCRYPT_ALLOC_PARA* memPara = nullptr;
	HWND hWnd = nullptr;

	SECURITY_STATUS status = NCryptProtectSecret(hProvider, NCRYPT_SILENT_FLAG, plaintextData, plaintextDataSize, memPara, hWnd, &ciphertext_ptr, &ciphertextLength);
	if (status != ERROR_SUCCESS) {
		std::cout << "NCryptProtectSecret failed with error code: " << status << std::endl;
		exit(1);
	}
}

void decryptData(const BYTE* ciphertextData, const ULONG ciphertextLength, BYTE* &plaintext_ptr, ULONG &plaintextLength) {
	SECURITY_STATUS status = NCryptUnprotectSecret(nullptr, NCRYPT_SILENT_FLAG, ciphertextData, ciphertextLength, nullptr, NULL, &plaintext_ptr, &plaintextLength);
	if (status != ERROR_SUCCESS) {
		std::cout << "NCryptUnprotectSecret failed with error code: " << status << std::endl;
		exit(1);
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
