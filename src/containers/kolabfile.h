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

#ifndef KOLABFILE_H
#define KOLABFILE_H

#include <string>
#include <vector>
#include <boost/scoped_ptr.hpp>
#include "kolabcontainers.h"
namespace Kolab {
    
    class File {
    public:
        File();
        ~File();
        File(const File &);
        void operator=(const File &);
        bool operator==(const File &) const;
        
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
        
        void setNote(const std::string &);
        std::string note() const;
        
        void setFile(const Attachment &);
        Attachment file() const;
        
        void setCustomProperties(const std::vector<CustomProperty> &);
        std::vector<CustomProperty> customProperties() const;
    private:
        struct Private;
        boost::scoped_ptr<Private> d;
    };

}

#endif

