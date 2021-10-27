#include "utils/SSLCertificate.h"

namespace RLMSocketServer
{
	SSLCertificate::SSLCertificate()
	{
		pkey = NULL;
		x509 = NULL;
	}

	SSLCertificate::~SSLCertificate()
	{
		cleanUp();
	}

	int SSLCertificate::generateCertification(std::string password,std::string keyPath,std::string certPath)
	{
		this->mPassword = password;
		this->mKeyPath	= keyPath;
		this->mCertPath	= certPath;
		int ret = mkCertification(2048,0,365);
		if (ret<0)
		{
			printf("certification error\n");
			return -1;
		}
		mkFiles();

		return 0;
	}

	int SSLCertificate::mkCertification(int bits, int serial, int days)
	{
		RSA			*rsa;
		X509		*x;
		EVP_PKEY	*pk;
		X509_NAME	*name=NULL;
		if (pkey == NULL)
		{
			if ((pk=EVP_PKEY_new()) == NULL)
				return -1;
		}
		else
			pk= pkey;
		if (x509 == NULL)
		{
			if ((x=X509_new()) == NULL)
				return -2;
		}
		else
			x= x509;
		rsa=RSA_generate_key(bits,RSA_F4,NULL,NULL);
		if (rsa==NULL)
			return -4;
		if (!EVP_PKEY_assign_RSA(pk,rsa))
			return -5;
		X509_set_version(x,2);
		ASN1_INTEGER_set(X509_get_serialNumber(x),serial);
		X509_gmtime_adj(X509_get_notBefore(x), 0);
		X509_gmtime_adj(X509_get_notAfter(x), (long)60*60*24*days);
		X509_set_pubkey(x,pk);
		name=X509_get_subject_name(x);
		X509_NAME_add_entry_by_txt(name, "C",  MBSTRING_ASC,(unsigned char *)"TN", -1, -1, 0);
		X509_NAME_add_entry_by_txt(name, "O",  MBSTRING_ASC,(unsigned char *)"MicroBuilders", -1, -1, 0);
		X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC,(unsigned char *)"RLM FixEngine Manager", -1, -1, 0);
		X509_set_issuer_name(x, name);
		X509_sign(x, pk, EVP_sha1());
		x509=x;
		pkey=pk;
		return 0;
	}

	int SSLCertificate::mkFiles()
	{
		FILE * f;
		f = fopen(mKeyPath.c_str(), "wb");
		//PEM_write_PrivateKey(f,pkey,EVP_des_ede3_cbc(),(unsigned char*)(mPassword.c_str()),mPassword.length(),NULL,NULL);
		PEM_write_PrivateKey(f,pkey,NULL,NULL,0,NULL, NULL);

		fclose(f);
		f = fopen(mCertPath.c_str(), "wb");
		PEM_write_X509(f,x509);
		fclose(f);
		return 0;
	}

	void SSLCertificate::cleanUp()
	{
		if (x509!=NULL)
			X509_free(x509);
		if (pkey!=NULL)
			EVP_PKEY_free(pkey);
		x509 = NULL;
		pkey = NULL;
	}
}
