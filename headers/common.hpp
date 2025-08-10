#pragma once
#include <GL/glew.h>
#include <algorithm>
#include <vector>
#include <string>
namespace mGLu
{
    typedef char *const typeID_t;
    template<typename T> 
    constexpr char const* typeID()
    {
        static const char t = 0;
        return &t;
    }

    constexpr const char* GetGLSLtype(unsigned int size, GLenum type)
    {
        switch(type)
        {
        case GL_FIXED:
        case GL_FLOAT:
            switch(size)
            {
            case 1:
                return "float";
            case 2:
                return "vec2";
            case 3:
                return "vec3";
            case 4:
                return "vec4";
            default:
                return nullptr;
            }
        case GL_DOUBLE:
            switch(size)
            {
            case 1:
                return "double";
            case 2:
                return "dvec2";
            case 3:
                return "dvec3";
            case 4:
                return "dvec4";
            default:
                return nullptr;
            }
        case GL_INT:
        case GL_SHORT:
        case GL_BYTE:
            switch(size)
            {
            case 1:
                return "int";
            case 2:
                return "ivec2";
            case 3:
                return "ivec3";
            case 4:
                return "ivec4";
            default:
                return nullptr;
            }
        case GL_UNSIGNED_INT:
        case GL_UNSIGNED_SHORT:
        case GL_UNSIGNED_BYTE:
            switch(size)
            {
            case 1:
                return "uint";
            case 2:
                return "uvec2";
            case 3:
                return "uvec3";
            case 4:
                return "uvec4";
            default:
                return nullptr;
            }
        
        default:
            return nullptr;
        }
    }
    class TreeNode
    {
    private:
        unsigned int inParentIndex = 0;
        unsigned int depth = 0;
        TreeNode *parent = nullptr, *root = this;
        std::vector<TreeNode*> children;
    public:
        TreeNode(TreeNode* _parent)
        {
            SetParent(_parent);
        }
        TreeNode(const TreeNode& other)
        {
            SetParent(other.parent);
        }
        ~TreeNode()
        {
            RebaseChildren(parent);
            SetParent(nullptr);
        }
        inline const std::vector<TreeNode*>& GetChildren() const
        {
            return children;
        }
        inline const TreeNode* GetRoot() const
        {
            return root;
        }
        inline const unsigned int GetDepth()
        {
            return depth;
        }
        static TreeNode* LCA(TreeNode *A, TreeNode *B)
        {
            if(A->root != B->root)
                return nullptr;
            while(A != B)
            {
                if(A->depth < B->depth)
                    B = B->parent;
                else
                    A = A->parent;
            }
            return A;
        }
        bool SetParent(TreeNode* newParent)
        {
            if(parent && newParent)
                if(LCA(this, newParent) == this)
                    return false;
            if(parent)
            {
                parent->children[inParentIndex] = parent->children.back();
                parent->children[inParentIndex]->inParentIndex = inParentIndex;
                parent->children.pop_back();
            }
            if(newParent)
            {
                inParentIndex = newParent->children.size();
                newParent->children.push_back(this);
                root = newParent->root;
                depth = newParent->depth + 1;
            }
            else
            {
                root = this;
                depth = 0;
            }
            parent = newParent;
            return true;
        }
        inline void RebaseChildren(TreeNode* newParent)
        {
            while(!children.empty())
                children.back()->SetParent(newParent);
        }
    };

    bool ReadTxtFile(const char* const path, std::string &out);
}
