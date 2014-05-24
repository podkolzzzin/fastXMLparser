#include <vector>
#include <string>
#include <stack>
using namespace std;
struct attribute
{
    string name;
    string value;
};
class xmlNode
{
    string tagName;
    string innerText;
    string afterTagText;
    vector<xmlNode*> innerTags;
    vector<attribute> attr;
    bool selfClose;
    void init(string tagName)
    {
        this->selfClose=false;
        this->tagName=tagName;
    }
    void destruct()
    {
        for(int i=0;i<innerTags.size();i++)
        {
            innerTags[i]->destruct();
            delete innerTags[i];
        }
    }
    int findAttribute(const string &name)
    {
        for(int i=0;i<this->attr.size();i++)
        {
            if(this->attr[i].name==name)
            {
                return i;
            }
        }
        return -1;
    }
public:
    xmlNode(const string &tagName)
    {
        init(tagName);
    }
    xmlNode(const char* tagName)
    {
        init(tagName);
    }
    void append(xmlNode &node)
    {
        innerTags.push_back(&node);
    }
    int numTags()
    {
        return innerTags.size();
    }
    void append(xmlNode *node)
    {
        innerTags.push_back(node);
    }
    void addAtribute(const string& name,const string& value)
    {
        attribute attr;
        attr.name=name;
        attr.value=value;
        addAtribute(attr);
    }
    void addAtribute(const char* name,const char* value)
    {
        string sName=name;
        string sValue=value;
        addAtribute(sName,sValue);
    }
    void addAtribute(const attribute &attr)
    {
        this->attr.push_back(attr);
    }

    bool setAttribute(const char* name,const char* value)
    {
        attribute attr;
        attr.name=name;
        attr.value=value;
        return setAttribute(attr);
    }
    bool setAttribute(const string &name,const string &value)
    {
        attribute attr;
        attr.name=name;
        attr.value=value;
        return setAttribute(attr);      
    }
    bool setAttribute(const attribute &attr)
    {
        int index=this->findAttribute(attr.name);
        if(index==-1) return false;
        this->attr[index].value=attr.value;
        return true;
    }

    string getAttribute(const string& name)
    {     
        int id=this->findAttribute(name);    
        if(id==-1)
        {
            return "";
        }
        return attr[id].value;
    }
    string getAttribute(const char* name)
    {
        string sName=name;
        return getAttribute(sName);
    }

    void setInnerText(const char* text)
    {
        string sText=text;
        setInnerText(sText);
    }
    void setInnerText(const string& text)
    {
        this->innerText=text;
    }
    void setCloseType(bool selfClose)
    {
        this->selfClose=selfClose;
    }
    void setAfterTagText(const string& text)
    {
        this->afterTagText=text;
    }
    void setAfterTagText(const char* text)
    {
        setAfterTagText(string(text));
    }
    string getInnerText()
    {
        return this->innerText;
    }
    string getAfterTagText()
    {
        return this->afterTagText;
    }
    string toString(int tab=0)
    {
        string rez;
        for(int j=0;j<tab;j++)
        {
                rez+=" ";
        }
        rez+="<";
        rez+=tagName;
        
        for(int i=0;i<attr.size();i++)
        {
            rez+=" ";
            rez+=attr[i].name;
            rez+="=";
            rez+="\"";
            rez+=attr[i].value;
            rez+="\"";
        } 
        if(this->selfClose)
        {
            rez+=" />";
            if(afterTagText.length()==0)
                return rez;
            else
            {
                rez+="\n";
                for(int j=0;j<tab;j++)
                {
                    rez+=" ";
                }
                rez+=afterTagText;                
            }
            return rez;
        }
        else 
            rez+=">";
        if(innerText!="")
        {
            rez+="\n"; 
            for(int j=0;j<tab+1;j++)
            {
                    rez+=" ";
            }
            rez+=innerText;
            
        }
        

        for(int i=0;i<innerTags.size();i++)
        {
            for(int j=0;j<tab+1;j++)
            {
                    rez+=" ";
            }
            rez+='\n';
            rez+=innerTags[i]->toString(tab+1);
        }
        rez+="\n";

        for(int j=0;j<tab;j++)
        {
                rez+=" ";
        }
        rez+="</";
        rez+=tagName;
        rez+=">";
        if(afterTagText!="")
        {
            rez+="\n";
            for(int j=0;j<tab;j++)
            {
                    rez+=" ";
            }
            //rez+='\n';
            rez+=afterTagText;
        }
        return rez;
    }

    xmlNode* getNodeByIndex(int index)
    {
        if(index<innerTags.size())
            return innerTags[index];
        return NULL;
    }
    vector<xmlNode*>* getNodeByAttributeValue(const string &attr,const string &value) // ищет только по непосредственно вложенным тегам
    {
        vector<xmlNode*>* rez=new vector<xmlNode*>();;
        for(int i=0;i<innerTags.size();i++)
        {
            if(innerTags[i]->getAttribute(attr)==value)
                rez->push_back(innerTags[i]);
        }
        return rez;
    } // 
    vector<xmlNode*>* getNodeByAttributeValue(const char* attr,const char* value) // ищет только по непосредственно вложенным тегам
    {
        return getNodeByAttributeValue(string(attr),string(value));
    }
    xmlNode* getNodeByAttributeValueUnique(const char* attr,const char* value)
    {
        return this->getNodeByAttributeValueUnique(string(attr),string(value));
    }
    xmlNode* getNodeByAttributeValueUnique(const string &attr,const string &value)
    {
        for(int i=0;i<innerTags.size();i++)
        {
            if(innerTags[i]->getAttribute(attr)==value)
                return innerTags[i];
        }
        return NULL;
    }
    xmlNode* findNodeByAttributeValueUnique(const string &attr,const string &value)
    {
        xmlNode* rez=getNodeByAttributeValueUnique(attr,value);
        if(rez) return rez;
        for(int i=0;i<innerTags.size();i++)
        {
            rez=innerTags[i]->findNodeByAttributeValueUnique(attr,value);
            if(rez) return rez;
        }
        return NULL;
    }
    xmlNode* findNodeByAttributeValueUnique(const char* attr,const char* value)
    {
        return findNodeByAttributeValueUnique(string(attr),string(value));
    }

    vector<xmlNode*>* findNodeByAttributeValue(const string &attr,const string &value,vector<xmlNode*>* rez=NULL)// ищет только по непосредственно вложенным тегам, а также всем вложенным
    {
        if(rez==NULL)
            vector<xmlNode*>* rez=new vector<xmlNode*>();
        vector<xmlNode*>* temp=getNodeByAttributeValue(attr,value);
        for(int i=0;i<temp->size();i++) 
            rez->push_back(temp->at(i));

        for(int i=0;i<innerTags.size();i++)
        {
            innerTags[i]->findNodeByAttributeValue(attr,value,rez);
        }
        return rez;
    }
    vector<xmlNode*>* findNodeByAttributeValue(const char* attr,const char* value) // ищет только по непосредственно вложенным тегам, а также всем вложенным
    {
        return findNodeByAttributeValue(string(attr),string(value));
    }

    xmlNode* getNodeByTagNameUnique(const string &tagName)
    {
        for(int i=0;i<innerTags.size();i++)
        {
            if(innerTags[i]->tagName==tagName) return innerTags[i]; 
        }
        return NULL;
    }
    xmlNode* getNodeByTagNameUnique(const char* tagName)
    {
        return getNodeByTagNameUnique(string(tagName));
    }

    xmlNode* findNodeByTagNameUnique(const string& tagName)
    {
        xmlNode* rez=getNodeByTagNameUnique(tagName);
        if(rez!=NULL) return rez;
        
        for(int i=0;i<innerTags.size();i++)
        {
            rez=innerTags[i]->findNodeByTagNameUnique(tagName);
            if(rez!=NULL) return rez;
        }
        return NULL;
    }
    xmlNode* findNodeByTagNameUnique(const char* tagName)
    {
        findNodeByTagNameUnique(string(tagName));
    }


    vector<xmlNode*>* getNodeByTagName(const string &tagName)
    {
        vector<xmlNode*>* rez=new vector<xmlNode*>();
        for(int i=0;i<innerTags.size();i++)
        {
            if(innerTags[i]->tagName==tagName) rez->push_back(innerTags[i]); 
        }      
        return rez;
    }
    vector<xmlNode*>* getNodeByTagName(const char* tagName)
    {
        getNodeByTagName(string(tagName));
    }

    vector<xmlNode*>* findNodeByTagName(const string &tagName,vector<xmlNode*>* rez=NULL)
    {
        if(rez==NULL)
            vector<xmlNode*>* rez=new vector<xmlNode*>();
        for(int i=0;i<innerTags.size();i++)
        {
            if(innerTags[i]->tagName==tagName) rez->push_back(innerTags[i]); 
            innerTags[i]->findNodeByTagName(tagName,rez);
        } 
        return rez;
    }
    vector<xmlNode*>* findNodeByTagName(const char* tagName)
    {
        return findNodeByTagName(string(tagName));
    }

    string getTagName()
    {
        return tagName;
    }
    ~xmlNode()
    {
        this->destruct();
    }
    friend class xmlDocument;
};
class XMLDocument
{
    xmlNode* head;
public:
    XMLDocument(const string& document)
    {
        createFromString(document);
    }
    xmlNode* getHead()
    {
        return head;
    }
    void createFromString(const string& str)
    {
        int length=str.length();
        string rez;
        int numWhite=0;
        bool insideTheTag=false;
        bool insideTheQuete=false;
        bool firstWhite=false;
        bool isCloseTag=false;
        bool insideTheAttr=false;
        bool insideTheAttrValue=false;
        bool queteAttr=false;
        bool valueStarted=false;
        bool onlyAddAttr=false;
        bool tagJustClosed=false;
        bool isWorkerTag=false;
        char queteType;
        string tagName="";

        
        stack<xmlNode*> prevTags;
        char closeWorkerTag;
        head=new xmlNode("headContentxmlDocumentData");
        xmlNode* currentNode=head;
        xmlNode* tNode,*prevNode=head;
        attribute tAttr;
        string text;

        int numMinus=0;
        char currentChar;
        for(int i=0;i<length;i++)
        {
            currentChar=str[i];
            if(isWorkerTag)
            {
                if(closeWorkerTag!='-')
                {
                    if(currentChar==closeWorkerTag)
                    {
                        if(closeWorkerTag!='>')
                            i++;
                        isWorkerTag=false;                        
                    }
                }
                else
                {
                    if(numMinus==4 && currentChar=='>')
                    {
                        isWorkerTag=false;
                        numMinus=0;
                    }
                    else if(currentChar!='>' && numMinus==4)
                        numMinus-=2;
                    else if(currentChar=='-')
                        numMinus++;
                    if(numMinus>2 && currentChar!='-') numMinus=2;
                                        
                }
                continue;
            }
            if(!insideTheTag)
            {               
                if(currentChar==' ' || currentChar=='\n' || currentChar=='\t')
                {
                    numWhite++;
                }
                else
                {
                    numWhite=0;
                    if(currentChar=='<') insideTheTag=true;
                    else if(currentChar=='\'' || currentChar=='"')
                    {
                        if(insideTheQuete)
                        {
                            if(currentChar==queteType)
                                insideTheQuete=false;
                        }
                        else
                        {
                            insideTheQuete=true;
                            queteType=currentChar;
                        }
                    }
                    if(!insideTheTag)
                        text+=currentChar;
                }
            }
            else
            {
                if(currentChar=='>')
                {
                    insideTheTag=false;
                    firstWhite=false;
                    if(insideTheAttrValue && tAttr.name.length()>0 && tAttr.value.length()>0)
                    {
                        currentNode->addAtribute(tAttr);
                        insideTheAttrValue=false;
                        insideTheAttr=false;
                        queteAttr=false;
                        valueStarted=false;
                        onlyAddAttr=false;
                    }
                    if(str[i-1]=='/')
                    {
                        if(currentNode->numTags()!=0)
                            prevNode->setAfterTagText(text);
                        currentNode->setCloseType(true);
                        prevNode=currentNode;
                        tagJustClosed=true;
                        currentNode=prevTags.top();
                        prevTags.pop();
                        tagName.clear();
                        text.clear();                        
                    }
                }
                if(insideTheTag)
                {
                    if(firstWhite)
                    {
                        if(currentChar!=' ' && currentChar!='\n' && currentChar!='\t')
                        {
                            insideTheAttr=true;
                        }
                        if(insideTheAttr)
                        {
                            if(currentChar=='=') 
                            {
                                insideTheAttrValue=true;
                                continue;
                            }
                            if(insideTheAttrValue)
                            {
                                if((currentChar=='\'' || currentChar=='"') && !valueStarted)
                                {
                                    queteAttr=true;
                                    queteType=currentChar;
                                    continue;
                                }
                                else if(valueStarted && queteAttr && (currentChar=='\'' || currentChar=='"'))
                                {
                                    valueStarted=false;
                                }
                                else if(!onlyAddAttr)
                                {
                                    valueStarted=true;
                                    tAttr.value+=currentChar;
                                }
                            }
                            else
                            {
                                tAttr.name+=currentChar;
                            }
                            if(queteAttr && insideTheAttrValue)
                            {
                                if(currentChar==queteType)
                                {
                                    valueStarted=false;
                                    queteAttr=false;
                                    onlyAddAttr=true;
                                    continue;
                                }
                            }
                            else if(insideTheAttrValue && currentChar==' ')
                            {
                                insideTheAttrValue=false;
                                queteAttr=false;
                                insideTheAttr=false;
                                valueStarted=false;
                                onlyAddAttr=false;
                                currentNode->addAtribute(tAttr);
                                tAttr.name.clear();
                                tAttr.value.clear();
                                continue;
                            }
                        }
                    }
                    else
                    {
                        if(currentChar!=' ' && currentChar!='\n' && currentChar!='\t')
                        {
                            if(tagName.length()==0 && currentChar=='?' || currentChar=='!')
                            {
                                isWorkerTag=true;
                                insideTheTag=false;
                                if(currentChar!='!') closeWorkerTag=currentChar;
                                else
                                {
                                    if(i+1<length)
                                    {
                                        if(str[i+1]=='-')
                                            closeWorkerTag='-';
                                        else 
                                            closeWorkerTag='>';
                                    }
                                }
                                continue;
                            }
                            tagName+=currentChar;
                        }
                        else
                        {
                            firstWhite=true;
                            prevTags.push(currentNode);
                            
                            tNode=new xmlNode(tagName);
                            currentNode->append(tNode);
                            if(tagJustClosed)
                            {
                                tagJustClosed=false;
                                prevNode->setAfterTagText(text);
                            }
                            else
                            {
                                if(text.length()>0)
                                    currentNode->setInnerText(text);
                            }
                            currentNode=tNode;
                            tagName.clear();
                            text.clear();
                        }
                    }
                }
                else
                {
                    if(tagName.length()>0)
                    { 
                        if(tagName[0]=='/')
                        {
                            if(text.length()>0 && currentNode->numTags()==0)
                                currentNode->setInnerText(text);
                            else
                                prevNode->setAfterTagText(text);
                            prevNode=currentNode;
                            tagJustClosed=true;

                            currentNode=prevTags.top();
                            prevTags.pop();
                            tagName.clear();
                            text.clear();
                        }
                        else
                        {
                            prevTags.push(currentNode);
                            tNode=new xmlNode(tagName);
                            currentNode->append(tNode);
                            if(tagJustClosed)
                            {
                                tagJustClosed=false;
                                prevNode->setAfterTagText(text);
                            }
                            else
                            {
                                if(text.length()>0)
                                    currentNode->setInnerText(text);
                            }
                            currentNode=tNode;
                            tagName.clear();
                            text.clear();
                        }
                    }
                }
            }
        }
        //cout<<head->toString();

        //cout<<rez<<endl;
    }
};