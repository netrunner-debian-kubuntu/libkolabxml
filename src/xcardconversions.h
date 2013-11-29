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

#ifndef KOLABXCARDCONVERSION_H
#define KOLABXCARDCONVERSION_H

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <iomanip>

#include <bindings/kolabformat-xcard.hxx>
#include <XMLParserWrapper.h>
#include "kolabcontainers.h"
#include "global_definitions.h"
#include "libkolabxml-version.h"
#include "utils.h"
#include "kolabcontact.h"
#include "shared_conversions.h"

namespace Kolab {
    namespace XCARD {
        
    const char* const XCARD_NAMESPACE = "urn:ietf:params:xml:ns:vcard-4.0";
    const char* const INDIVIDUAL = "individual";
    const char* const GROUP = "group";
    const char* const MIME_PGP_KEYS = "application/pgp-keys";
    const char* const MIME_PKCS7_MIME = "application/pkcs7-mime";

using namespace Kolab::Utils;
    
template <typename T> 
std::string getType();

template <> 
std::string getType<Kolab::Contact>()
{
    return INDIVIDUAL;
}

template <> 
std::string getType<Kolab::DistList>()
{
    return GROUP;
}

template <typename T>
xsd::cxx::tree::sequence<T> fromList(const std::vector<std::string> &input)
{
    xsd::cxx::tree::sequence<T> list;
    BOOST_FOREACH(const std::string &s, input) {
        list.push_back(T(s));
    }
    return list;
}

template <typename T>
xsd::cxx::tree::sequence<T> fromList(const std::vector<std::string> &input, int preferredIndex)
{
    xsd::cxx::tree::sequence<T> list;
    int index = 0;
    BOOST_FOREACH(const std::string &s, input) {
        T im(s);
        if(preferredIndex == index) {
            typename T::parameters_type parameters;
            parameters.baseParameter().push_back(vcard_4_0::prefParamType(vcard_4_0::prefParamType::integer_default_value()));
            im.parameters(parameters);
        }
        index++;
        list.push_back(im);
    }
    return list;
}

template <typename T>
std::vector<std::string> toUriList(const xsd::cxx::tree::sequence<T> &input)
{
    std::vector<std::string> list;
    BOOST_FOREACH(const vcard_4_0::UriPropertyType &s, input) {
        list.push_back(s.uri());
    }
    return list;
}

template <typename T>
std::vector<std::string> toTextList(const xsd::cxx::tree::sequence<T> &input)
{
    std::vector<std::string> list;
    BOOST_FOREACH(const vcard_4_0::TextPropertyType &s, input) {
        list.push_back(s.text());
    }
    return list;
}

std::vector<std::string> toStringList(const ::xsd::cxx::tree::sequence< ::xml_schema::string > &s)
{
    std::vector<std::string> d;
    std::copy(s.begin(), s.end(), std::back_inserter(d));
    return d;
}

::xsd::cxx::tree::sequence< ::xml_schema::string > fromStringList(const std::vector<std::string> &s)
{
    ::xsd::cxx::tree::sequence< ::xml_schema::string > d;
    std::copy(s.begin(), s.end(), std::back_inserter(d));
    return d;
}

std::string fromDate(const cDateTime &dt)
{
    if (!dt.isDateOnly()) {
        WARNING("fromDate called on date time value");
    }
    std::stringstream s;
    s.fill('0');
    s.width(4);
    s << dt.year();
    s.width(2);
    s << dt.month();
    s.width(2);
    s << dt.day();
    return s.str();
}

std::string fromDateTime(const cDateTime &dt)
{
    std::stringstream s;
    s.fill('0');
    s << std::right; 
    s.width(4);
    s << dt.year();
    s.width(2);
    s << dt.month();
    s.width(2);
    s << dt.day();
    s.width(1);
    if (dt.isDateOnly()) {
        return s.str();
    }
    s << "T";
    s.width(2);
    s << dt.hour();
    s.width(2);
    s << dt.minute();
    s.width(2);
    s << dt.second();
    if (dt.isUTC()) {
        s << "Z";
    }
    return s.str();
}

cDateTime toDateTime(const std::string &input)
{
    int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
    bool isUtc = false;
    try {
        year = boost::lexical_cast<int>(input.substr(0, 4));
        month = boost::lexical_cast<int>(input.substr(4, 2));
        day = boost::lexical_cast<int>(input.substr(6, 2));
        if (input.size() >= 15) { //Minimum for time
            if (input.at(8) != 'T') {
                ERROR("Wrong demiliter");
                return cDateTime();
            }
            hour = boost::lexical_cast<int>(input.substr(9, 2));
            minute = boost::lexical_cast<int>(input.substr(11, 2));
            second = boost::lexical_cast<int>(input.substr(13, 2));
        } else {
            if (input.size() >= 9) {
                ERROR("Invalid dt " + input);
                return cDateTime();
            }
            return cDateTime(year, month, day);
        }
        if (input.size() >= 16) {
            if (input.at(15) == 'Z') {
                isUtc = true;
            } else {
                ERROR("wrong utc char? " + input.at(15));
                return cDateTime();
            }
        }
    } catch (boost::bad_lexical_cast &c) {
        ERROR("failed to convert: "+std::string(c.what()));
        return cDateTime();
    } catch (...) {
        ERROR("failed to convert: unknown exception");
        return cDateTime();
    }
    return cDateTime(year, month, day, hour, minute, second, isUtc);
}

cDateTime toDateTime(const vcard_4_0::DateDatetimePropertyType &prop)
{
    if (prop.date_time()) {
        return toDateTime(*prop.date_time());
    } else if (prop.date()) {
        return toDateTime(*prop.date());
    }
    ERROR("no date and no datetime");
    return cDateTime();
}

template <typename T>
T fromDateTime(const Kolab::cDateTime &dt)
{
    T prop;

    if (dt.isDateOnly()) {
        prop.date(fromDate(dt));
    } else {
        prop.date_time(fromDateTime(dt));
    }
    return prop;
}

vcard_4_0::PrefTypeValueType fromCryptoPref(Kolab::Crypto::CryptoPref pref)
{
    switch (pref) {
        case Kolab::Crypto::Always:
            return vcard_4_0::CryptoType::encryptpref_type::text_type::Always;
        case Kolab::Crypto::Ask:
            return vcard_4_0::CryptoType::encryptpref_type::text_type::Ask;
        case Kolab::Crypto::IfPossible:
            return vcard_4_0::CryptoType::encryptpref_type::text_type::IfPossible;
        case Kolab::Crypto::Never:
            return vcard_4_0::CryptoType::encryptpref_type::text_type::Never;
        default:
            WARNING("unknown encrypt pref");
    }
    return vcard_4_0::CryptoType::encryptpref_type::text_type::Ask;
}

Kolab::Crypto::CryptoPref toCryptoPref(vcard_4_0::PrefTypeValueType pref)
{
    switch (pref) {
        case vcard_4_0::CryptoType::encryptpref_type::text_type::Always:
            return Kolab::Crypto::Always;
        case vcard_4_0::CryptoType::encryptpref_type::text_type::Ask:
            return Kolab::Crypto::Ask;
        case vcard_4_0::CryptoType::encryptpref_type::text_type::IfPossible:
            return Kolab::Crypto::IfPossible;
        case vcard_4_0::CryptoType::encryptpref_type::text_type::Never:
            return Kolab::Crypto::Never;
        default:
            WARNING("unknown encrypt pref");
    }
    return Kolab::Crypto::Ask;
}

vcard_4_0::relatedPropType fromRelated(const Kolab::Related &r)
{
    using namespace vcard_4_0;
    vcard_4_0::relatedPropType related;
    if (r.type() == Kolab::Related::Uid) {
        related.uri(r.uri());
    } else {
        related.text(r.text());
    }
    if (r.relationTypes() != Kolab::Related::NoRelation) {
        
        vcard::adr_type::parameters_type::baseParameter_sequence base;
        vcard::adr_type::parameters_type b;
        
        vcard_4_0::typeParamType::text_sequence seq;
        if (r.relationTypes() & Kolab::Related::Child) {
            seq.push_back(TypeValueType::child);
        }
        if (r.relationTypes() & Kolab::Related::Spouse) {
            seq.push_back(TypeValueType::spouse);
        }
        if (r.relationTypes() & Kolab::Related::Assistant) {
            seq.push_back(TypeValueType::x_assistant);
        }
        if (r.relationTypes() & Kolab::Related::Manager) {
            seq.push_back(TypeValueType::x_manager);
        }
        if (!seq.empty()) {
            vcard_4_0::typeParamType type;
            type.text(seq);
            b.baseParameter().push_back(type);
        }
        
        related.parameters(b);
    }
    
    return related;
}

Kolab::Related toRelated(const vcard_4_0::relatedPropType &r)
{
    Kolab::Related::DescriptionType type = Kolab::Related::Invalid;
    std::string textOrUri;
    if (r.uri()) {
        type = Kolab::Related::Uid;
        textOrUri = *r.uri();
    } else if (r.text()) {
        type = Kolab::Related::Text;
        textOrUri = *r.text();
    } else {
        ERROR("no text and no uri");
        return Kolab::Related();
    }
    Kolab::Related related(type, textOrUri);
    if (r.parameters()) {
        BOOST_FOREACH(const vcard_4_0::ArrayOfParameters::baseParameter_type &param, (*r.parameters()).baseParameter()) {
            if (const vcard_4_0::typeParamType *rel = dynamic_cast<const vcard_4_0::typeParamType*> (&param)) {
                int types = 0;
                BOOST_FOREACH(const std::string &s, rel->text()) {
                    if (s == vcard_4_0::TypeValueType(vcard_4_0::TypeValueType::child)) {
                        types |= Kolab::Related::Child;
                    }
                    if (s == vcard_4_0::TypeValueType(vcard_4_0::TypeValueType::spouse)) {
                        types |= Kolab::Related::Spouse;
                    }
                    if (s == vcard_4_0::TypeValueType(vcard_4_0::TypeValueType::x_assistant)) {
                        types |= Kolab::Related::Assistant;
                    }
                    if (s == vcard_4_0::TypeValueType(vcard_4_0::TypeValueType::x_manager)) {
                        types |= Kolab::Related::Manager;
                    }
                }
                related.setRelationTypes(types);
            } 
        }
    }
    return related;
}

vcard_4_0::vcard::adr_type fromAddress(const Kolab::Address &address)
{
    using namespace vcard_4_0;
    vcard::adr_type a(vcard::adr_type::pobox_type(std::string()/*address.pobox()*/), 
                      vcard::adr_type::ext_type(std::string()/*address.ext()*/),
                                                vcard::adr_type::street_type(address.street()),
                                                vcard::adr_type::locality_type(address.locality()),
                                                vcard::adr_type::region_type(address.region()),
                                                address.code(),
                                                vcard::adr_type::country_type(address.country())
                      );
    vcard::adr_type::parameters_type b;
    if (address.types()) {
        vcard_4_0::typeParamType::text_sequence seq;
        if (address.types() & Kolab::Address::Home) {
            seq.push_back(TypeValueType::home);
        }
        if (address.types() & Kolab::Address::Work) {
            seq.push_back(TypeValueType::work);
        }
        if (!seq.empty()) {
            vcard_4_0::typeParamType type;
            type.text(seq);
            b.baseParameter().push_back(type);
        }
    }
    if (!address.label().empty()) {
        b.baseParameter().push_back(vcard_4_0::labelParamType(address.label()));
    }
    a.parameters(b);
    return a;
}

Kolab::Address toAddress(const vcard_4_0::vcard::adr_type &adr, bool *isPreferred = 0)
{
    using namespace vcard_4_0;
    Address address;
    if (adr.parameters()) {
        BOOST_FOREACH(const vcard_4_0::ArrayOfParameters::baseParameter_type &param, (*adr.parameters()).baseParameter()) {
            if (const vcard_4_0::labelParamType *rel = dynamic_cast<const vcard_4_0::labelParamType*> (&param)) {
                address.setLabel(rel->text());
            } else if (isPreferred && dynamic_cast<const vcard_4_0::prefParamType*> (&param)) {
                *isPreferred = true;
            } else if (const vcard_4_0::typeParamType *rel = dynamic_cast<const vcard_4_0::typeParamType*> (&param)) {
                int types = 0;
                BOOST_FOREACH(const std::string &s, rel->text()) {
                    if (s == TypeValueType(TypeValueType::work)) {
                        types |= Kolab::Telephone::Work;
                    }
                    if (s == TypeValueType(TypeValueType::home)) {
                        types |= Kolab::Telephone::Home;
                    }
                }
                address.setTypes(types);
            } 
        }
    }
    
    address.setCode(adr.code());
    address.setCountry(adr.country());
    address.setLocality(adr.locality());
    address.setRegion(adr.region());
    address.setStreet(adr.street());
    return address;
}

std::string toGeoUri(double lat, double lon)
{
    //lexical_cast doesn't work, so we're using stringstream
    std::stringstream s;
    s << "geo:";
    s.precision(15); //can't use std::numeric_limits<double>::max_digits10 because that's c++ 0x, it should be 17, but that seems to cause rounding problems... no idea why.
    s << lat << ",";
    s.precision(15); //Needed to get the right precision somehow...
    s << lon;
    return s.str();
}

bool fromGeoUri(const std::string &uri, double &lat, double &lon)
{
    if (uri.substr(0,4) != std::string("geo:")) {
        WARNING("not a geo uri");
        return false;
    }
    std::size_t pos1 = uri.find(",");
    if (pos1 == std::string::npos) {
        WARNING("not a valid geo uri");
        return false;
    }
    lat = boost::lexical_cast<double>(uri.substr(4, pos1-4));
    lon = boost::lexical_cast<double>(uri.substr(pos1+1, uri.size()-pos1-1));
    return true;
}

template <typename T> 
void writeCard(vcard_4_0::vcard &vcard, const T &);

template <> 
void writeCard<Kolab::Contact>(vcard_4_0::vcard &vcard, const Kolab::Contact &contact)
{
    using namespace vcard_4_0;
    
    if (!contact.categories().empty()) {
        vcard_4_0::vcard::categories_type cat;
        vcard_4_0::vcard::categories_type::text_sequence seq;
        const std::vector<std::string> &l = contact.categories();
        BOOST_FOREACH(const std::string &s, l) {
            seq.push_back(s);
        }
        cat.text(seq);
        vcard.categories(cat);
    }
    
    if (contact.nameComponents().isValid()) {
        const NameComponents &nc = contact.nameComponents();
        vcard::n_type n;
        n.surname(fromStringList(nc.surnames()));
        n.given(fromStringList(nc.given()));
        n.additional(fromStringList(nc.additional()));
        n.prefix(fromStringList(nc.prefixes()));
        n.suffix(fromStringList(nc.suffixes()));
        vcard.n(n);
    }
    
    if (!contact.note().empty()) {
        vcard.note(vcard::note_type(contact.note()));
    }
    
    if (!contact.freeBusyUrl().empty()) {
        vcard.fburl(vcard::fburl_type(contact.freeBusyUrl()));
    }
    
    if (!contact.titles().empty()) {
        vcard.title(fromList<vcard::title_type>(contact.titles()));
    }
    
    if (!contact.affiliations().empty()) {
        vcard::group_sequence affiliations;
        const std::vector<Affiliation> &l = contact.affiliations();
        BOOST_FOREACH(const Affiliation &a, l) {
            if (a == Affiliation()) { //skip empty ones
                LOG("skipped empty affiliation");
                continue;
            }
            affiliationPropType::org_type org;
            org.text().push_back(a.organisation());
            const std::vector<std::string> &orgUnits = a.organisationalUnits();
            BOOST_FOREACH(const std::string &unit, orgUnits) {
                org.text().push_back(unit);
            }
            vcard::group_type group(org);
            if (!a.logo().empty()) {
                group.logo(affiliationPropType::logo_type(uriInlineEncoding(a.logo(), a.logoMimetype())));
            }
            group.role(fromList<affiliationPropType::role_type>(a.roles()));
            const std::vector<Related> &relateds = a.relateds();
            BOOST_FOREACH(const Related &rel, relateds) {
                group.related().push_back(fromRelated(rel));
            }
            const std::vector<Address> &addersses = a.addresses();
            BOOST_FOREACH(const Address &adr, addersses) {
                group.adr().push_back(fromAddress(adr));
            }

            affiliations.push_back(group);
        }
        vcard.group(affiliations);
    }
    
    if (!contact.urls().empty()) {
        vcard_4_0::vcard::url_sequence urls;
        const std::vector<Url> &l = contact.urls();
        BOOST_FOREACH(const Kolab::Url &url, l) {
            vcard::url_type u(url.url());
            if (url.type() == Kolab::Url::Blog) {
                vcard::adr_type::parameters_type b;
                vcard_4_0::typeParamType p;
                p.text().push_back(vcard_4_0::TypeValueType::x_blog);
                b.baseParameter().push_back(p);
                u.parameters(b);
            }
            urls.push_back(u);
        }
        vcard.url(urls);
    }
    
    if (!contact.addresses().empty()) {
        vcard::adr_sequence adrs;
        int index = 0;
        const std::vector<Address> &l = contact.addresses();
        BOOST_FOREACH(const Kolab::Address &address, l) {
            vcard::adr_type a = fromAddress(address);
            if(contact.addressPreferredIndex() == index) {
                if (a.parameters()) {
                    (*a.parameters()).baseParameter().push_back(vcard_4_0::prefParamType(vcard_4_0::prefParamType::integer_default_value()));
                } else {
                    vcard::adr_type::parameters_type b;
                    b.baseParameter().push_back(vcard_4_0::prefParamType(vcard_4_0::prefParamType::integer_default_value()));
                    a.parameters(b);
                }
            }
            index++;
            adrs.push_back(a);
        }
        vcard.adr(adrs);
    }
    
    if (!contact.nickNames().empty()) {
        vcard::nickname_type::text_sequence textsequence;
        const std::vector<std::string> &l = contact.nickNames();
        BOOST_FOREACH(const std::string &s, l) {
            textsequence.push_back(s);
        }
        vcard::nickname_type nickName;
        nickName.text(textsequence);
        vcard.nickname(nickName);
    }
    
    if (!contact.relateds().empty()) {
        vcard::related_sequence seq;
        const std::vector<Related> &l = contact.relateds();
        BOOST_FOREACH(const Kolab::Related &r, l) {
            seq.push_back(fromRelated(r));
        }
        vcard.related(seq);
    }
    
    if (contact.bDay().isValid()) {
        Kolab::cDateTime dt = contact.bDay();
        if (dt.isUTC() || !dt.timezone().empty()) {
            WARNING("Must be local time, local time assumed");
            dt.setUTC(false);
        }
        vcard.bday(fromDateTime<vcard::bday_type>(dt));
    }
    
    if (contact.anniversary().isValid()) {
        Kolab::cDateTime dt = contact.anniversary();
        if (dt.isUTC() || !dt.timezone().empty()) {
            WARNING("Must be local time, local time assumed");
            dt.setUTC(false);
        }
        vcard.anniversary(fromDateTime<vcard::anniversary_type>(dt));
    }
    
    if (!contact.photo().empty()) {
        vcard::photo_type photo(vcard_4_0::UriPropertyType::uri_type(uriInlineEncoding(contact.photo(), contact.photoMimetype())));
        vcard.photo(photo);
    }
    
    if (contact.gender() != Contact::NotSet) {
        switch (contact.gender()) {
            case Contact::NotSpecified:
                vcard.gender(vcard::gender_type(vcard::gender_type::sex_type::empty));
                break;
            case Contact::Male:
                vcard.gender(vcard::gender_type(vcard::gender_type::sex_type::M));
                break;
            case Contact::Female:
                vcard.gender(vcard::gender_type(vcard::gender_type::sex_type::F));
                break;
            default:
                ERROR("Unhandled gender");
        }
    }
    
    if (!contact.languages().empty()) {
        vcard.lang(fromList<vcard::lang_type>(contact.languages()));
    }
    
    if (!contact.telephones().empty()) {
        vcard::tel_sequence seq;
        int index = 0;
        const std::vector<Telephone> &l = contact.telephones();
        BOOST_FOREACH(const Kolab::Telephone &t, l) {
            vcard::tel_type tel(t.number());
            vcard_4_0::typeParamType telTypeParam;
            if (t.types() & Kolab::Telephone::Car) {
                telTypeParam.text().push_back(TypeValueType::x_car);
            } 
            if (t.types() & Kolab::Telephone::Cell) {
                telTypeParam.text().push_back(TypeValueType::cell);
            } 
            if (t.types() & Kolab::Telephone::Fax) {
                telTypeParam.text().push_back(TypeValueType::fax);
            } 
            if (t.types() & Kolab::Telephone::Home) {
                telTypeParam.text().push_back(TypeValueType::home);
            } 
            if (t.types() & Kolab::Telephone::Work) {
                telTypeParam.text().push_back(TypeValueType::work);
            } 
            if (t.types() & Kolab::Telephone::Text) {
                telTypeParam.text().push_back(TypeValueType::text);
            } 
            if (t.types() & Kolab::Telephone::Voice) {
                telTypeParam.text().push_back(TypeValueType::voice);
            } 
            if (t.types() & Kolab::Telephone::Video) {
                telTypeParam.text().push_back(TypeValueType::video);
            } 
            if (t.types() & Kolab::Telephone::Textphone) {
                telTypeParam.text().push_back(TypeValueType::textphone);
            } 
            if (t.types() & Kolab::Telephone::Pager) {
                telTypeParam.text().push_back(TypeValueType::pager);
            }
            vcard::tel_type::parameters_type params;
            if(contact.telephonesPreferredIndex() == index) {
                params.baseParameter().push_back(vcard_4_0::prefParamType(vcard_4_0::prefParamType::integer_default_value()));
            }
            index++;

            if (!telTypeParam.text().empty()) {
                params.baseParameter().push_back(telTypeParam);
                tel.parameters(params);
            }
            seq.push_back(tel);
        }
        vcard.tel(seq);
    }
    
    if (!contact.imAddresses().empty()) {
        vcard.impp(fromList<vcard::impp_type>(contact.imAddresses(), contact.imAddressPreferredIndex()));
    }
    
    if (!contact.emailAddresses().empty()) {
        vcard::email_sequence seq;
        int index = 0;
        const std::vector<Email> &l = contact.emailAddresses();
        BOOST_FOREACH(const Kolab::Email &e, l) {
            vcard::email_type email(e.address());
            vcard_4_0::typeParamType emailTypeParam;
            if (e.types() & Kolab::Email::Home) {
                emailTypeParam.text().push_back(TypeValueType::home);
            } 
            if (e.types() & Kolab::Email::Work) {
                emailTypeParam.text().push_back(TypeValueType::work);
            } 
            vcard::tel_type::parameters_type params;
            if (!emailTypeParam.text().empty()) {
                params.baseParameter().push_back(emailTypeParam);
            }
            if(contact.emailAddressPreferredIndex() == index) {
                params.baseParameter().push_back(vcard_4_0::prefParamType(vcard_4_0::prefParamType::integer_default_value()));
            }
            index++;

            if (!params.baseParameter().empty()) {
                email.parameters(params);
            }
            seq.push_back(email);
        }
        vcard.email(seq);
    }
    
    if (!contact.gpsPos().empty()) {
        vcard_4_0::vcard::geo_sequence list;
        const std::vector<Geo> &l = contact.gpsPos();
        BOOST_FOREACH(const Kolab::Geo &g, l) {
            list.push_back(vcard_4_0::vcard::geo_type(toGeoUri(g.latitude, g.longitude)));
        }
        vcard.geo(list);
    }
    
    if (contact.crypto().isValid()) {
        vcard::x_crypto_type crypto;
        const Kolab::Crypto &c = contact.crypto();
        if (c.allowed()) {
            vcard::x_crypto_type::allowed_type::text_sequence seq;
            if (c.allowed() & Kolab::Crypto::PGPinline) {
                seq.push_back(vcard::x_crypto_type::allowed_type::text_type::PGP_INLINE);
            }
            if (c.allowed() & Kolab::Crypto::PGPmime) {
                seq.push_back(vcard::x_crypto_type::allowed_type::text_type::PGP_MIME);
            }
            if (c.allowed() & Kolab::Crypto::SMIME) {
                seq.push_back(vcard::x_crypto_type::allowed_type::text_type::S_MIME);
            }
            if (c.allowed() & Kolab::Crypto::SMIMEopaque) {
                seq.push_back(vcard::x_crypto_type::allowed_type::text_type::S_MIMEOpaque);
            }
            vcard::x_crypto_type::allowed_type allowed;
            allowed.text(seq);
            crypto.allowed(allowed);
        }
        
        crypto.encryptpref(fromCryptoPref(c.encryptPref()));
        crypto.signpref(fromCryptoPref(c.signPref()));

        vcard.x_crypto(crypto);
    }
    
    if (!contact.keys().empty()) {
        vcard_4_0::vcard::key_sequence keys;
        const std::vector<Key> &l = contact.keys();
        BOOST_FOREACH (const Kolab::Key &k, l) {
            switch (k.type()) {
                case Kolab::Key::PGP:
                    keys.push_back(vcard_4_0::keyPropType(uriInlineEncoding(k.key(), MIME_PGP_KEYS)));
                    break;
                case Kolab::Key::PKCS7_MIME:
                    keys.push_back(vcard_4_0::keyPropType(uriInlineEncoding(k.key(), MIME_PKCS7_MIME)));
                    break;
                default:
                    LOG("Unhandled/Invalid keytype");
                    break;
            }
        }

        if (!keys.empty()) {
            vcard.key(keys);
        }
    }


}

template <> 
void writeCard<Kolab::DistList>(vcard_4_0::vcard &vcard, const Kolab::DistList &distlist)
{
    if (!distlist.members().empty()) {
        vcard_4_0::vcard::member_sequence members;
        const std::vector<ContactReference> &l = distlist.members();
        BOOST_FOREACH (const Kolab::ContactReference &m, l) {
            if (!m.uid().empty()) {
                members.push_back(vcard_4_0::vcard::member_type(Shared::toURN(m.uid())));
            } else {
                members.push_back(vcard_4_0::vcard::member_type(Shared::toMailto(m.email(), m.name())));
            }
        }
        vcard.member(members);
    }
}



template <typename T>
std::string serializeCard(const T &card, const std::string prod = std::string()) {

    using namespace vcard_4_0;
    
    clearErrors();

    try {
        vcard_4_0::vcard::uid_type uid(Shared::toURN(getUID(card.uid())));
        setCreatedUid(Shared::fromURN(uid.uri()));
        vcard_4_0::vcard::x_kolab_version_type kolab_version(KOLAB_FORMAT_VERSION);
        vcard_4_0::vcard::prodid_type prodid(getProductId(prod));
        vcard_4_0::vcard::rev_type rev(fromDateTime(timestamp()));
        vcard_4_0::vcard::kind_type kind(getType<T>());
        vcard_4_0::vcard::fn_type fn(card.name());

        vcard_4_0::vcard vcard(uid, kolab_version, prodid, rev, kind, fn);
        
        writeCard<T>(vcard, card);
        
        if (!card.customProperties().empty()) {
            const std::vector<Kolab::CustomProperty> &l = card.customProperties(); 
            BOOST_FOREACH(const Kolab::CustomProperty &a, l) {
                vcard.x_custom().push_back(vcard_4_0::CustomType(a.identifier, a.value));
            }
        }
        
        VcardsType vcards(vcard);

        xml_schema::namespace_infomap map;
        map[""].name = XCARD_NAMESPACE;
        
        std::ostringstream ostringstream;
        vcard_4_0::vcards(ostringstream, vcards, map);
        return ostringstream.str();
    } catch  (const xml_schema::exception& e) {
        std::cerr <<  e << std::endl;
    } catch (...) {
        CRITICAL("Unhandled exception");
    }
    CRITICAL("Failed to write vcard!");
    return std::string();
}

template <typename T>
boost::shared_ptr<T> readCard(const vcard_4_0::VcardsType::vcard_type &vcard);

template <>
boost::shared_ptr<Kolab::Contact> readCard <Kolab::Contact> (const vcard_4_0::VcardsType::vcard_type &vcard)
{
    using namespace vcard_4_0;
    boost::shared_ptr<Kolab::Contact> contact(new Kolab::Contact);
    
    if (vcard.categories()) {
        contact->setCategories(toStringList((*vcard.categories()).text()));
    }
    
    if (vcard.n()) {
        NameComponents nc;
        nc.setSurnames(toStringList((*vcard.n()).surname()));
        nc.setGiven(toStringList((*vcard.n()).given()));
        nc.setPrefixes(toStringList((*vcard.n()).prefix()));
        nc.setSuffixes(toStringList((*vcard.n()).suffix()));
        nc.setAdditional(toStringList((*vcard.n()).additional()));
        contact->setNameComponents(nc);
    }
    if (vcard.note()) {
        contact->setNote((*vcard.note()).text());
    }
    if (vcard.fburl()) {
        contact->setFreeBusyUrl((*vcard.fburl()).uri());
    }
    if (!vcard.title().empty()) {
        contact->setTitles(toTextList<vcard::title_type >(vcard.title()));
    }
    if (!vcard.group().empty()) {
        std::vector<Kolab::Affiliation> list;
        BOOST_FOREACH (const vcard::group_type &group, vcard.group()) {
            Kolab::Affiliation aff;
            if (!group.org().text().empty()) {
                aff.setOrganisation(*group.org().text().begin());
                std::vector<std::string> units;
                for ( vcard_4_0::NonEmptyTextListPropertyType::text_const_iterator it = ++group.org().text().begin(); it != group.org().text().end(); it++) {
                    units.push_back(*it);
                }
                aff.setOrganisationalUnits(units);
            } else {
                WARNING("No org present");
            }
            std::string mimetype;
            if (group.logo()) {
                const std::string &logo = uriInlineDecoding((*group.logo()).uri(), mimetype);
                aff.setLogo(logo, mimetype);
            }
            aff.setRoles(toTextList<vcard::group_type::role_type>(group.role()));
            std::vector<Related> relateds;
            BOOST_FOREACH(const vcard::group_type::related_type &rel, group.related()) {
                relateds.push_back(toRelated(rel));
            }
            aff.setRelateds(relateds);
            std::vector<Address> addresses;
            BOOST_FOREACH(const vcard::group_type::adr_type &adr, group.adr()) {
                addresses.push_back(toAddress(adr));
            }
            aff.setAddresses(addresses);
            list.push_back(aff);
        }
        contact->setAffiliations(list);
    }
    if (!vcard.url().empty()) {
        std::vector<Kolab::Url> urls;
        BOOST_FOREACH(const vcard_4_0::vcard::url_type &url, vcard.url()) {
            if (url.parameters()) { //We have only one fixed parameter (x-blog)
                urls.push_back(Kolab::Url(url.uri(), Kolab::Url::Blog));
            } else {
                urls.push_back(Kolab::Url(url.uri()));
            }
        }
        contact->setUrls(urls);
    }
    if (!vcard.adr().empty()) {
        std::vector<Kolab::Address> list;
        int preferredIndex = -1;
        int index = 0;
        BOOST_FOREACH(const vcard::adr_type &adr, vcard.adr()) {
            bool isPreferred = false;
            const Kolab::Address &address = toAddress(adr, &isPreferred);
            if (isPreferred) {
                preferredIndex = index;
            }
            index++;
            list.push_back(address);
        }
        contact->setAddresses(list, preferredIndex);
    }
    if (vcard.nickname()) {
        contact->setNickNames(toTextList<TextListPropertyType::text_type>((*vcard.nickname()).text()));
    }
    if (!vcard.related().empty()) {
        std::vector<Related> list;
        BOOST_FOREACH(const vcard_4_0::vcard::related_type &r, vcard.related()) {
            list.push_back(toRelated(r));
        }
        contact->setRelateds(list);
    }
    if (vcard.bday()) {
        contact->setBDay(toDateTime(*vcard.bday()));
    }
    if (vcard.anniversary()) {
        contact->setAnniversary(toDateTime(*vcard.anniversary()));
    }
    if (vcard.photo()) {
        std::string mimetype;
        const std::string decodedPhoto =  uriInlineDecoding((*vcard.photo()).uri(), mimetype);
        contact->setPhoto(decodedPhoto, mimetype);
    }
    if (vcard.gender()) {
        if ((*vcard.gender()).sex() == vcard::gender_type::sex_type::empty) {
            contact->setGender(Kolab::Contact::NotSpecified);
        } else if ((*vcard.gender()).sex() == vcard::gender_type::sex_type::M) {
            contact->setGender(Kolab::Contact::Male);
        } else if ((*vcard.gender()).sex() == vcard::gender_type::sex_type::F) {
            contact->setGender(Kolab::Contact::Female);
        }
    }
    if (!vcard.lang().empty()) {
        std::vector<std::string> list;
        BOOST_FOREACH(const vcard::lang_type l, vcard.lang()) {
            list.push_back(l.language_tag());
        }
        contact->setLanguages(list);
    }
    if (!vcard.tel().empty()) {
        std::vector<Kolab::Telephone> list;
        int preferredIndex = -1;
        int index = 0;
        BOOST_FOREACH(const vcard::tel_type &tel, vcard.tel()) {
            Kolab::Telephone telephone;
            if (tel.parameters()) {
                BOOST_FOREACH(const vcard_4_0::ArrayOfParameters::baseParameter_type &param, (*tel.parameters()).baseParameter()) {
                    if (dynamic_cast<const vcard_4_0::prefParamType*> (&param)) {
                        preferredIndex = index;
                    } else if (const vcard_4_0::typeParamType *rel = dynamic_cast<const vcard_4_0::typeParamType*> (&param)) {
                        int types = 0;
                        BOOST_FOREACH(const std::string &s, rel->text()) {
                            if (s == TypeValueType(TypeValueType::work)) {
                                types |= Kolab::Telephone::Work;
                            }
                            if (s == TypeValueType(TypeValueType::home)) {
                                types |= Kolab::Telephone::Home;
                            }
                            if (s == TypeValueType(TypeValueType::text)) {
                                types |= Kolab::Telephone::Text;
                            }
                            if (s == TypeValueType(TypeValueType::voice)) {
                                types |= Kolab::Telephone::Voice;
                            }
                            if (s == TypeValueType(TypeValueType::fax)) {
                                types |= Kolab::Telephone::Fax;
                            }
                            if (s == TypeValueType(TypeValueType::cell)) {
                                types |= Kolab::Telephone::Cell;
                            }
                            if (s == TypeValueType(TypeValueType::video)) {
                                types |= Kolab::Telephone::Video;
                            }
                            if (s == TypeValueType(TypeValueType::pager)) {
                                types |= Kolab::Telephone::Pager;
                            }
                            if (s == TypeValueType(TypeValueType::textphone)) {
                                types |= Kolab::Telephone::Textphone;
                            }
                            if (s == TypeValueType(TypeValueType::x_car)) {
                                types |= Kolab::Telephone::Car;
                            }
                        }
                        telephone.setTypes(types);
                    } 
                }
            }
            index++;

            telephone.setNumber(tel.text());
            list.push_back(telephone);
        }
        contact->setTelephones(list, preferredIndex);
    }
    
    if (!vcard.impp().empty()) {
        int preferredIndex = -1;
        
        std::vector<std::string> list;
        int i = 0;
        BOOST_FOREACH(const vcard_4_0::UriPropertyType &s, vcard.impp()) {
            if (s.parameters()) {
                BOOST_FOREACH(const vcard_4_0::ArrayOfParameters::baseParameter_type &param, (*s.parameters()).baseParameter()) {
                    if (dynamic_cast<const vcard_4_0::prefParamType*> (&param)) {
                        preferredIndex = i;
                    } 
                }
            }
            i++;
            list.push_back(s.uri());
        }
        
        contact->setIMaddresses(list, preferredIndex);
    }
    if (!vcard.email().empty()) {
        int preferredIndex = -1;
        
        std::vector<Kolab::Email> list;
        int i = 0;
        BOOST_FOREACH(const vcard_4_0::TextPropertyType &s, vcard.email()) {
            Kolab::Email email;
            if (s.parameters()) {
                BOOST_FOREACH(const vcard_4_0::ArrayOfParameters::baseParameter_type &param, (*s.parameters()).baseParameter()) {
                    if (dynamic_cast<const vcard_4_0::prefParamType*> (&param)) {
                        preferredIndex = i;
                    } else if (const vcard_4_0::typeParamType *rel = dynamic_cast<const vcard_4_0::typeParamType*> (&param)) {
                        int types = 0;
                        BOOST_FOREACH(const std::string &s, rel->text()) {
                            if (s == TypeValueType(TypeValueType::work)) {
                                types |= Kolab::Email::Work;
                            }
                            if (s == TypeValueType(TypeValueType::home)) {
                                types |= Kolab::Email::Home;
                            }
                        }
                        email.setTypes(types);
                    }
                }
            }
            i++;
            email.setAddress(s.text());
            list.push_back(email);
        }
        
        contact->setEmailAddresses(list, preferredIndex);
    }
    if (!vcard.geo().empty()) {
        std::vector<Geo> list;
        BOOST_FOREACH(const vcard_4_0::geoPropType &s, vcard.geo()) {
            double lon = 0.0;
            double lat = 0.0;
            if (fromGeoUri(s.uri(), lat, lon)) {
                list.push_back(Kolab::Geo(lat, lon));
            }
        }
        contact->setGPSpos(list);
    }
    if (vcard.x_crypto()) {
        const vcard_4_0::vcard::x_crypto_type &crypto = *vcard.x_crypto();
        Kolab::Crypto c;
        if (crypto.allowed()) {
            int allowed = 0;
            BOOST_FOREACH(const vcard_4_0::vcard::x_crypto_type::allowed_type::text_type &m, crypto.allowed()->text()) {
                if (m == vcard::x_crypto_type::allowed_type::text_type::PGP_INLINE) {
                    allowed |= Kolab::Crypto::PGPinline;
                } else if (m == vcard::x_crypto_type::allowed_type::text_type::PGP_MIME) {
                    allowed |= Kolab::Crypto::PGPmime;
                } else if (m == vcard::x_crypto_type::allowed_type::text_type::S_MIME) {
                    allowed |= Kolab::Crypto::SMIME;
                } else if (m == vcard::x_crypto_type::allowed_type::text_type::S_MIMEOpaque) {
                    allowed |= Kolab::Crypto::SMIMEopaque;
                } else {
                    WARNING("unknown allowed property");
                }
            }
            c.setAllowed(allowed);
        }
        
        if (crypto.encryptpref()) {
            c.setEncryptPref(toCryptoPref(crypto.encryptpref()->text()));
        }
        if (crypto.signpref()) {
            c.setSignPref(toCryptoPref(crypto.signpref()->text()));
        }
        contact->setCrypto(c);
    }
    
    if (!vcard.key().empty()) {
        std::string mimetype;
        std::vector<Kolab::Key> keys;
        BOOST_FOREACH(const vcard_4_0::keyPropType &k, vcard.key()) {                
            const std::string &key = uriInlineDecoding(k.uri(), mimetype);
            if (mimetype == MIME_PGP_KEYS) {
                keys.push_back(Kolab::Key(key, Kolab::Key::PGP));
            } else if (mimetype == MIME_PKCS7_MIME) {
                keys.push_back(Kolab::Key(key, Kolab::Key::PKCS7_MIME));
            } else {
                WARNING("wrong mimetype on key");
            }
        }
        contact->setKeys(keys);
    }
    
    return contact;
}

template <>
boost::shared_ptr<Kolab::DistList> readCard <Kolab::DistList> (const vcard_4_0::VcardsType::vcard_type &vcard)
{
    using namespace vcard_4_0;
    boost::shared_ptr<Kolab::DistList> distlist(new Kolab::DistList);

    if (!vcard.member().empty()) {
        std::vector<Kolab::ContactReference> members;

        BOOST_FOREACH(const vcard_4_0::vcard::member_type & m, vcard.member()) {
            members.push_back(Shared::toContactReference(m.uri()));
        }
        distlist->setMembers(members);
    }
    return distlist;
}

template <typename T>
boost::shared_ptr<T> deserializeCard(const std::string& s, bool isUrl)
{
    clearErrors();
    try {
        std::auto_ptr<vcard_4_0::VcardsType> vcards;
        if (isUrl) {
            xsd::cxx::xml::dom::auto_ptr <xercesc::DOMDocument > doc = XMLParserWrapper::inst().parseFile(s);
            if (doc.get()) {
                vcards = vcard_4_0::vcards(doc);
            }
        } else {
            xsd::cxx::xml::dom::auto_ptr <xercesc::DOMDocument > doc = XMLParserWrapper::inst().parseString(s);
            if (doc.get()) {
                vcards = vcard_4_0::vcards(doc);
            }
        }

        if (!vcards.get()) {
            CRITICAL("failed to parse card!");
            return boost::shared_ptr<T>();
        }
        
        boost::shared_ptr<T> card = readCard<T>(vcards->vcard());
        card->setUid(Shared::fromURN(vcards->vcard().uid().uri()));
        card->setName(vcards->vcard().fn().text());
        card->setLastModified(toDateTime(vcards->vcard().rev().timestamp()));
        
        setProductId( vcards->vcard().prodid().text() );
//         setFormatVersion( vcards->vcard().version().text() );
//         global_xCardVersion = vcalendar.properties().version().text();
        setKolabVersion( vcards->vcard().x_kolab_version().text() );
        
        if (!vcards->vcard().x_custom().empty()) {
            std::vector<Kolab::CustomProperty> customProperties;
            BOOST_FOREACH(const vcard_4_0::CustomType &p, vcards->vcard().x_custom()) {
                customProperties.push_back(CustomProperty(p.identifier(), p.value()));
            }
            card->setCustomProperties(customProperties);
        }
        
        return card;
    } catch  (const xml_schema::exception& e) {
        std::cerr <<  e << std::endl;
    } catch (...) {
        CRITICAL("Unhandled exception");
    }
    CRITICAL("Failed to read vcard!");
    return boost::shared_ptr<T>();
}

    }
    
} //Namespace

#endif
