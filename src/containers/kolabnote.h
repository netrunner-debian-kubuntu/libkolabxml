/*
 * Copyright (C) 2012  Christian Mollekopf <mollekopf@kolabsys.com>
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

#ifndef KOLABNOTE_H
#define KOLABNOTE_H

#include <string>
#include <vector>
#include <boost/scoped_ptr.hpp>
#include "kolabcontainers.h"
namespace Kolab {
    
    class Note {
    public:
        Note();
        ~Note();
        Note(const Note &);
        void operator=(const Note &);
        bool operator==(const Note &) const;
        
        bool isValid() const;
        
        void setUid(const std::string &);
        std::string uid() const;
        
        void setCreated(const cDateTime &);
        cDateTime created() const;
        
        void setLastModified(const cDateTime &);
        cDateTime lastModified() const;
        
        void setClassification(Classification);
        Classification classification() const;
        
        void setCategories(const std::vector<std::string> &);
        void addCategory(const std::string &);
        std::vector<std::string> categories() const;
        
        void setSummary(const std::string &);
        std::string summary() const;
        
        void setDescription(const std::string &);
        std::string description() const;
        
        void setColor(const std::string &);
        std::string color() const;
        
        void setAttachments(const std::vector<Attachment> &);
        std::vector<Attachment> attachments() const;
        
        void setCustomProperties(const std::vector<CustomProperty> &);
        std::vector<CustomProperty> customProperties() const;
    private:
        struct Private;
        boost::scoped_ptr<Private> d;
    };

}

#endif

