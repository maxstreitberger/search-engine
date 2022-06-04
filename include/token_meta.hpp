#ifndef TOKEN_META_HPP
#define TOKEN_META_HPP

#include "doc_meta.hpp"

namespace tokenmeta {
    struct TokenMeta {
        TokenMeta() : document_id{0}, num_appearances{0}, doc_ptr{nullptr} {};
        TokenMeta(int doc_id) : document_id{doc_id}, doc_ptr{nullptr} {};
        TokenMeta(int doc_id, int num_appearances, std::vector<int> positions) : document_id{doc_id}, num_appearances{num_appearances}, positions{positions} {};
        TokenMeta(int doc_id , int position, const docmeta::DocumentMeta* ptr) : document_id{doc_id}, num_appearances{1}, positions{position}, doc_ptr{ptr} {};
        TokenMeta(int doc_id, int num_appearances, std::vector<int> positions, const docmeta::DocumentMeta* ptr) : document_id{doc_id}, num_appearances{num_appearances}, positions{positions}, doc_ptr{ptr}{};

        friend std::ostream& operator<<(std::ostream& os, const TokenMeta& meta) {
            os << " { document_id: " << meta.document_id << ',' << " num_appearances: " << meta.num_appearances << ',' << " positions: [ "; 
            for (int position: meta.positions) {
                os << position << ", ";
            }
            os << " ], ";
            os << "doc_ptr: " << meta.doc_ptr << " }";
            return os << std::endl;
        }

        bool operator< ( const TokenMeta &r ) const {
            return document_id < r.document_id;
        }

        bool operator==(const TokenMeta& rhs) const {
            return document_id == rhs.document_id && doc_ptr == rhs.doc_ptr && positions == rhs.positions && num_appearances == rhs.num_appearances;
        }

        int document_id;
        const docmeta::DocumentMeta* doc_ptr;
        std::vector<int> positions;
        int num_appearances;
    };
}

#endif