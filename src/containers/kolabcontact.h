/*
 * Copyright (C) 2011  Christian Mollekopf <mollekopf@kolabsys.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KOLABCONTACT_H
#define KOLABCONTACT_H

#include <string>
#include <vector>
#include <boost/scoped_ptr.hpp>
#include "kolabcontainers.h"

namespace Kolab {
    
struct NameComponents {
    bool operator==(const NameComponents &other) const { return mSurnames == other.mSurnames &&
                                                        mGiven == other.mGiven &&
                                                        mAdditional == other.mAdditional &&
                                                        mPrefixes == other.mPrefixes &&
                                                        mSuffixes == other.mSuffixes;
                                                        };
    void setSurnames(const std::vector<std::string> &s) { mSurnames = s; };
    std::vector<std::string> surnames() const { return mSurnames; };
    void setGiven(const std::vector<std::string> &s) { mGiven = s; };
    std::vector<std::string> given() const { return mGiven; };
    void setAdditional(const std::vector<std::string> &s) { mAdditional = s; };
    std::vector<std::string> additional() const { return mAdditional; };
    void setPrefixes(const std::vector<std::string> &s) { mPrefixes = s; };
    std::vector<std::string> prefixes() const { return mPrefixes; };
    void setSuffixes(const std::vector<std::string> &s) { mSuffixes = s; };
    std::vector<std::string> suffixes() const { return mSuffixes; };
    bool isValid() const { return !(mSurnames.empty() && mGiven.empty() && mAdditional.empty() && mPrefixes.empty() && mSuffixes.empty()); };
private:
    std::vector<std::string> mSurnames;
    std::vector<std::string> mGiven;
    std::vector<std::string> mAdditional;
    std::vector<std::string> mPrefixes;
    std::vector<std::string> mSuffixes;
};

struct Related {
    
    enum DescriptionType {
        Invalid,
        Text,
        Uid
    };
    Related(): mType(Invalid), mRelationType(NoRelation) {};
    Related(DescriptionType t, const std::string &textOrUri, int relationType = NoRelation)
    :   mType(t), mRelationType(relationType)
    {
        if (t == Text) {
            mText = textOrUri;
        } else {
            mUri = textOrUri;
        }
    };
    bool operator==(const Related &other) const { return mType == other.mType &&
    mUri == other.mUri &&
    mText == other.mText &&
    mRelationType == other.mRelationType;
    };
    DescriptionType type() const { return mType; };
    std::string uri() const { return mUri; };
    std::string text() const { return mText; };
    enum RelationType {
        NoRelation = 0,
        Child = 0x01,
        Spouse = 0x02,
        Manager = 0x04,
        Assistant = 0x08,
    };
    void setRelationTypes(int t) { mRelationType = t; };
    int relationTypes() const { return mRelationType; };
private:
    DescriptionType mType;
    std::string mUri;
    std::string mText;
    int mRelationType;
};

struct Address {
    
    Address(): mTypes(0) {};
    enum Type {
        Work = 0x01,
        Home = 0x02
    };
    bool operator==(const Address &other) const { return mTypes == other.mTypes &&
    mLabel == other.mLabel &&
    mStreet == other.mStreet &&
    mLocality == other.mLocality &&
    mRegion == other.mRegion &&
    mCode == other.mCode &&
    mCountry == other.mCountry;
    };
    void setTypes(int t) { mTypes = t; };
    int types() const { return mTypes; };
    
    void setLabel(const std::string &s) { mLabel = s; };
    std::string label() const { return mLabel; }
    
    void setStreet(const std::string &s) { mStreet = s; };
    std::string street() const { return mStreet; };
    
    void setLocality(const std::string &s) { mLocality = s; };
    std::string locality() const { return mLocality; };
    
    void setRegion(const std::string &s) { mRegion = s; };
    std::string region() const { return mRegion; };
    
    void setCode(const std::string &s) { mCode = s; };
    std::string code() const { return mCode; };
    
    void setCountry(const std::string &s) { mCountry = s; };
    std::string country() const { return mCountry; };
private:
    int mTypes;
    std::string mLabel;
    std::string mStreet;
    std::string mLocality;
    std::string mRegion;
    std::string mCode;
    std::string mCountry;
};

struct Affiliation {
    bool operator==(const Affiliation &other) const { return mOrg == other.mOrg &&
                                                    mOrgUnits == other.mOrgUnits &&
                                                    mLogo == other.mLogo &&
                                                    mLogoMimetype == other.mLogoMimetype &&
                                                    mRoles == other.mRoles &&
                                                    mRelateds == other.mRelateds &&
                                                    mOffices == other.mOffices;
                                                    };
    void setOrganisation(const std::string &org) { mOrg = org; };
    std::string organisation() const { return mOrg; };
    void setOrganisationalUnits(const std::vector<std::string> &units) { mOrgUnits = units; };
    std::vector<std::string> organisationalUnits() const { return mOrgUnits; };
    void setLogo(const std::string &l, const std::string mimetype) { mLogo = l; mLogoMimetype = mimetype; };
    std::string logo() const { return mLogo; };
    std::string logoMimetype() const { return mLogoMimetype; };

    void setRoles(const std::vector<std::string> &roles) { mRoles = roles; };
    std::vector<std::string> roles() const { return mRoles; };
    void setRelateds(const std::vector<Related> &relateds) { mRelateds = relateds; };
    std::vector<Related> relateds() const { return mRelateds; };
    void setAddresses(const std::vector<Address> &offices) { mOffices = offices; };
    std::vector<Address> addresses() const { return mOffices; };
private:
    std::string mOrg;
    std::vector<std::string> mOrgUnits;
    std::string mLogo;
    std::string mLogoMimetype;
    std::vector<std::string> mRoles;
    std::vector<Related> mRelateds;
    std::vector<Address> mOffices;
};

struct Telephone {
    enum Type {
        Work = 0x01,
        Home = 0x02,
        Text = 0x04,
        Voice = 0x08,
        Fax = 0x10,
        Cell = 0x20,
        Video = 0x40,
        Pager = 0x80,
        Textphone = 0x100,
        Car = 0x200
    };
    Telephone(): mType(0){};
    bool operator==(const Telephone &other) const { return mNumber == other.mNumber &&
                                                    mType == other.mType;
                                                    };
    void setTypes(int t) { mType = t; };
    int types() const { return mType; };
    void setNumber(const std::string &n) { mNumber = n; };
    std::string number() const { return mNumber; };
private:
    std::string mNumber;
    int mType;
};

struct Crypto {
    enum CryptoTypes {
        PGPinline = 0x01,
        PGPmime = 0x02,
        SMIME = 0x04,
        SMIMEopaque = 0x08
    };
    
    enum CryptoPref {
        Ask,
        Never,
        Always,
        IfPossible
    };
    Crypto(): mCryptoTypes(0), mSignPref(Ask), mEncryptPref(Ask){};
    bool operator==(const Crypto &other) const { return mCryptoTypes == other.mCryptoTypes &&
                                                    mSignPref == other.mSignPref &&
                                                    mEncryptPref == other.mEncryptPref; };
    bool isValid() const { return mCryptoTypes; };

    void setAllowed(int cryptoTypes) { mCryptoTypes = cryptoTypes; };
    int allowed() const { return mCryptoTypes; };
    void setSignPref(CryptoPref p) { mSignPref = p; };
    CryptoPref signPref() const { return mSignPref; };
    void setEncryptPref(CryptoPref p) { mEncryptPref = p; };
    CryptoPref encryptPref() const { return mEncryptPref; };
private:
    int mCryptoTypes;
    CryptoPref mSignPref;
    CryptoPref mEncryptPref;
};

struct Geo {
    Geo(): latitude(0.0), longitude(0.0) {};
    Geo(double lat, double lon)
    : latitude(lat), longitude(lon) {};
    
    bool operator==(const Geo &other) const{ return (longitude == other.longitude && latitude == other.latitude);};
    double latitude;
    double longitude;
};

struct Url {
    enum UrlTypes {
        NoType = 0,
        Blog
    };
    Url(): mType(NoType) {};
    Url(const std::string &u, int t = NoType): mUrl(u), mType(t) {};
    
    bool operator==(const Url &other) const{ return (mType == other.mType && mUrl == other.mUrl);};
    
    int type() const { return mType; };
    std::string url() const { return mUrl; };
private:
    std::string mUrl;
    int mType;
};

struct Key {
    enum KeyType {
        Invalid,
        PGP,
        PKCS7_MIME
    };
    Key(): keytype(Invalid) {};
    Key(const std::string &k, KeyType type)
    : mKey(k), keytype(type) {};
    
    bool operator==(const Key &other) const{ return (mKey == other.mKey && keytype == other.keytype);};
    
    KeyType type() const { return keytype; };
    std::string key() const { return mKey; };
    
private:
    std::string mKey;
    KeyType keytype;
};


class DistList {
public:
    DistList();
    ~DistList();
    DistList(const DistList &);
    void operator=(const DistList &);

    bool isValid() const;

    void setUid(const std::string &);
    std::string uid() const;

    void setLastModified(const cDateTime &);
    cDateTime lastModified() const;

    void setName(const std::string &);
    std::string name() const;

    void setMembers(const std::vector<ContactReference> &);
    std::vector<ContactReference> members() const;

    void setCustomProperties(const std::vector<CustomProperty> &);
    std::vector<CustomProperty> customProperties() const;

private:
    struct Private;
    boost::scoped_ptr<Private> d;
};

class Contact {
public:
    Contact();
    ~Contact();
    Contact(const Contact &);
    void operator=(const Contact &);

    bool isValid() const;

    void setUid(const std::string &);
    std::string uid() const;
    
    void setLastModified(const cDateTime &);
    cDateTime lastModified() const;
    
    void setCategories(const std::vector<std::string> &);
    void addCategory(const std::string &);
    std::vector<std::string> categories() const;
    
    void setName(const std::string &);
    std::string name() const;
    
    void setNameComponents(const NameComponents &);
    NameComponents nameComponents() const;
    
    void setNote(const std::string &);
    std::string note() const;
    
    void setFreeBusyUrl(const std::string &);
    std::string freeBusyUrl() const;
    
    void setTitles(const std::vector<std::string> &titles);
    std::vector<std::string> titles() const;
    
    void setAffiliations(const std::vector<Affiliation> &);
    std::vector<Affiliation> affiliations() const;
    
    void setUrls(const std::vector<Url> &);
    std::vector<Url> urls() const;
    
    void setAddresses(const std::vector<Address> &, int preferred = -1);
    std::vector<Address> addresses() const;
    int addressPreferredIndex() const;
    
    void setNickNames(const std::vector< std::string > &);
    std::vector< std::string > nickNames() const;
    
    void setRelateds(const std::vector<Related> &);
    std::vector<Related> relateds() const;
     
    void setBDay(const cDateTime &);
    cDateTime bDay() const;
    
    void setAnniversary(const cDateTime &);
    cDateTime anniversary() const;
    
    void setPhoto(const std::string &data, const std::string &mimetype);
    std::string photo() const;
    std::string photoMimetype() const;
    
    enum Gender {
        NotSet,
        NotSpecified,
        Male,
        Female
    };
    
    void setGender(Gender);
    Gender gender() const;
    
    void setLanguages(const std::vector<std::string> &);
    std::vector<std::string> languages() const;
    
    void setTelephones(const std::vector<Telephone> &, int preferredIndex = -1);
    std::vector<Telephone> telephones() const;
    int telephonesPreferredIndex() const;
    
    void setIMaddresses(const std::vector<std::string> &, int preferredIndex = -1);
    std::vector<std::string> imAddresses() const;
    int imAddressPreferredIndex() const;
    
    void setEmailAddresses(const std::vector<std::string> &, int preferredIndex = -1);
    std::vector<std::string> emailAddresses() const;
    int emailAddressPreferredIndex() const;
    
    void setGPSpos(const std::vector<Geo> &);
    std::vector<Geo> gpsPos() const;
    
    void setKeys(const std::vector<Key> &);
    std::vector<Key> keys() const;
    
    void setCrypto(const Crypto &);
    Crypto crypto() const;
    
    void setCustomProperties(const std::vector<CustomProperty> &);
    std::vector<CustomProperty> customProperties() const;

private:
    struct Private;
    boost::scoped_ptr<Private> d;
};

} //Namespace

#endif // KOLABCONTACT_H
