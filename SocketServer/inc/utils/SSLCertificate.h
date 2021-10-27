#ifndef SSL_CERTIFICATE_H_
#define SSL_CERTIFICATE_H_

#include <pem.h>
#include <conf.h>
#include <x509v3.h>
#include <iostream>

namespace RLMSocketServer
{
	/*!
	 * \brief SSL certification class, used to generate a new SSL/TLS certification
	 *
	 * SSL sessions needs a certificate, this class will generate a new one on the fly.
	 */
	class SSLCertificate
	{
	public:
								SSLCertificate();
		virtual					~SSLCertificate();

		/**
		 * generates a new certification and stores the key.pem and cert.pem on the specified path
		 * @param password : certification password.
		 * @param keyPath  : certification key path.
		 * @param certPath : public and private certification path.
		 * @return 0 on success <0 on errors
		*/
		int						generateCertification(std::string password,std::string keyPath,std::string certPath);
		/**
		 * clean up function. this needs to be called by the class destructor
		*/
		void					cleanUp();

	private:
		/**
		 * @param bits 		: rsa encoding bits.
		 * @param serial  	: serial number of the certification.
		 * @param days		: number of days to be valid .
		 * @return 0 on success <0 on errors
		 * \note
		 * called by @ref generateCertification
		 * 2048 encoding and 365 day validity are used
		*/
		int						mkCertification(int bits, int serial, int days);
		/**
		 * writes the certification to the files.
		 * @return always returns 0
		*/
		int						mkFiles();

		EVP_PKEY 				*pkey;
		X509 					*x509;
		std::string				mPassword,mKeyPath,mCertPath;

	};
}

#endif
