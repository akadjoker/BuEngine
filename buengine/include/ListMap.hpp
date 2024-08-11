

#pragma once

template <class T>
class ListMap
{
protected:
    class HashedItem
    {
    public:
        union
        {
            const void *m_ptrIndex;
            char *m_strIndex;
            u32 m_intIndex;
        };
        HashedItem *m_pNextItem;
        T m_pItem;
        unsigned int m_iItemStrLength;

        HashedItem()
        {
            m_iItemStrLength = 0;
            m_ptrIndex = 0;
            m_pItem = 0;
            m_pNextItem = 0;
        }
        ~HashedItem()
        {
            if (m_iItemStrLength)
                delete[] m_strIndex;
        }
    };

    HashedItem **m_pHashedItems = 0;
    HashedItem *m_pIter = 0;
    u32 m_iIterIndex = 0;
    u32 m_iListSize = 0;
    u32 m_iLastID = 100000;
    u32 m_iItemCount = 0;
    u32 m_iPower = 0;

    u32 HashIndexInt(u32 iIndex) const;
    u32 HashIndexStr(const char *szIndex, unsigned int *pLength = 0) const;
    u32 HashIndexPtr(const void *ptr) const;

public:
    ListMap(u32 iSize = 1024);
    ~ListMap();
    void Resize(u32 iSize);

    void Clone(ListMap<T> *pOther);

    void AddItem(T pItem, u32 iID);
    T GetItem(u32 iID) const;
    T GetFirst();
    T GetNext();
    T RemoveItem(u32 iID);
    u32 GetFreeID(u32 max = 0x7fffffff) const;
    void ClearAll();
    u32 GetCount() const { return m_iItemCount; }

    // string versions
    void AddItem(T pItem, const char *szID);
    T GetItem(const char *szID) const;
    T RemoveItem(const char *szID);

    // pointer versions
    void AddItem(T pItem, const void *ptr);
    T GetItem(const void *ptr) const;
    T RemoveItem(const void *ptr);
};

template <class T>
ListMap<T>::ListMap(u32 iSize)
{
    if (iSize < 2)
        iSize = 2;
    if (iSize > 0x7fffffff)
        iSize = 0x7fffffff;

    // find the next highest power of 2
    u32 iNewSize = 1;
    m_iPower = 0;
    while (iNewSize < iSize)
    {
        iNewSize = iNewSize << 1;
        m_iPower++;
    }
    m_iListSize = iNewSize;

    m_pHashedItems = new HashedItem *[m_iListSize];
    for (u32 i = 0; i < m_iListSize; i++)
    {
        m_pHashedItems[i] = 0;
    }
}

template <class T>
ListMap<T>::~ListMap()
{
    ClearAll();
    delete[] m_pHashedItems;
}

template <class T>
void ListMap<T>::Resize(u32 iSize)
{
    if (m_pHashedItems)
    {
        ClearAll();
        delete[] m_pHashedItems;
    }

    if (iSize < 2)
        iSize = 2;
    if (iSize > 0x7fffffff)
        iSize = 0x7fffffff;

    // find the next highest power of 2
    u32 iNewSize = 1;
    m_iPower = 0;
    while (iNewSize < iSize)
    {
        iNewSize = iNewSize << 1;
        m_iPower++;
    }
    m_iListSize = iNewSize;

    m_pHashedItems = new HashedItem *[m_iListSize];
    for (u32 i = 0; i < m_iListSize; i++)
    {
        m_pHashedItems[i] = 0;
    }

    m_pIter = 0;
    m_iIterIndex = 0;

    m_iLastID = 100000;
    m_iItemCount = 0;
}

template <class T>
void ListMap<T>::Clone(ListMap<T> *pOther)
{
    m_pIter = pOther->m_pIter;
    m_iIterIndex = pOther->m_iIterIndex;
    m_iLastID = pOther->m_iLastID;
    m_iItemCount = pOther->m_iItemCount;
    m_iPower = pOther->m_iPower;
    m_iListSize = pOther->m_iListSize;
    if (pOther->m_pHashedItems)
        m_pHashedItems = new HashedItem *[m_iListSize];

    for (u32 i = 0; i < pOther->m_iListSize; i++)
    {
        m_pHashedItems[i] = 0;

        HashedItem *pCurrItem = pOther->m_pHashedItems[i];
        while (pCurrItem)
        {
            HashedItem *pNewItem = new HashedItem();
            *pNewItem = *pCurrItem;
            if (pNewItem->m_iItemStrLength > 0)
            {
                pNewItem->m_strIndex = new char[pNewItem->m_iItemStrLength + 1];
                memcpy(pNewItem->m_strIndex, pCurrItem->m_strIndex, pNewItem->m_iItemStrLength + 1);
            }
            pNewItem->m_pNextItem = m_pHashedItems[i];
            m_pHashedItems[i] = pNewItem;

            pCurrItem = pCurrItem->m_pNextItem;
        }
    }
}

template <class T>
u32 ListMap<T>::HashIndexInt(u32 iIndex) const
{
    iIndex = (iIndex >> 16) ^ iIndex;
    iIndex = (iIndex >> m_iPower) ^ iIndex;
    return iIndex & (m_iListSize - 1); // m_iListSize must be a power of 2
}

template <class T>
u32 ListMap<T>::HashIndexStr(const char *szIndex, unsigned int *pLength) const
{
    u32 iIndex = 2166136261;
    while (*szIndex)
    {
        iIndex ^= *szIndex;
        iIndex *= 16777619;
        szIndex++;
        if (pLength)
            (*pLength)++;
    }

    iIndex = (iIndex >> 16) ^ iIndex;
    iIndex = (iIndex >> m_iPower) ^ iIndex;
    return iIndex & (m_iListSize - 1); // m_iListSize must be a power of 2
}

template <class T>
u32 ListMap<T>::HashIndexPtr(const void *ptr) const
{
    unsigned long long ptr64 = (unsigned long long)ptr;
    unsigned int ptr32 = (unsigned int)ptr64;
    ptr32 = (ptr64 >> 32) ^ ptr32;
    ptr32 = (ptr32 >> 16) ^ ptr32;
    ptr32 = (ptr32 >> m_iPower) ^ ptr32;
    return ptr32 & (m_iListSize - 1); //  must be a power of 2
}

template <class T>
void ListMap<T>::AddItem(T pItem, u32 iID)
{

    u32 index = HashIndexInt(iID);

    HashedItem *pNewItem = new HashedItem();
    pNewItem->m_iItemStrLength = 0;
    pNewItem->m_intIndex = iID;
    pNewItem->m_pItem = pItem;

    pNewItem->m_pNextItem = m_pHashedItems[index];
    m_pHashedItems[index] = pNewItem;

    if (iID > m_iLastID)
        m_iLastID = iID;
    if (m_iLastID > 0x7fffffff)
        m_iLastID = 100000;

    m_iItemCount++;
}

template <class T>
T ListMap<T>::GetItem(u32 iID) const
{
    if (m_iItemCount == 0)
        return 0;

    u32 index = HashIndexInt(iID);
    HashedItem *pItem = m_pHashedItems[index];
    while (pItem)
    {
        if (pItem->m_intIndex == iID)
            return pItem->m_pItem;
        pItem = pItem->m_pNextItem;
    }
    return 0;
}

template <class T>
T ListMap<T>::GetFirst()
{
    if (m_iItemCount == 0)
        return 0;

    T pReturnItem = 0;
    m_pIter = 0;
    m_iIterIndex = 0;
    for (u32 i = 0; i < m_iListSize; i++)
    {
        HashedItem *pCurrItem = m_pHashedItems[i];
        while (pCurrItem)
        {
            if (!pReturnItem)
                pReturnItem = pCurrItem->m_pItem;
            else
            {
                // find next ready for GetNext()
                m_pIter = pCurrItem;
                m_iIterIndex = i;
                return pReturnItem;
            }

            pCurrItem = pCurrItem->m_pNextItem;
        }
    }

    return pReturnItem;
}

template <class T>
T ListMap<T>::GetNext()
{
    if (!m_pIter)
        return 0;

    T pReturnItem = m_pIter->m_pItem;
    if (m_pIter->m_pNextItem)
    {
        m_pIter = m_pIter->m_pNextItem;
        return pReturnItem;
    }

    // find next item for next GetNext()
    for (u32 i = m_iIterIndex + 1; i < m_iListSize; i++)
    {
        if (m_pHashedItems[i])
        {
            m_pIter = m_pHashedItems[i];
            m_iIterIndex = i;
            return pReturnItem;
        }
    }

    m_pIter = 0;
    m_iIterIndex = 0;
    return pReturnItem;
}

template <class T>
T ListMap<T>::RemoveItem(u32 iID)
{
    u32 index = HashIndexInt(iID);
    HashedItem *pItem = m_pHashedItems[index];
    HashedItem *pLast = 0;
    T pItemContent = 0;

    while (pItem)
    {
        if (pItem->m_intIndex == iID)
        {
            // if this item is the current iterator then re-calculate the next step
            if (pItem == m_pIter)
                GetNext();

            // remove the item from the list
            if (pLast)
                pLast->m_pNextItem = pItem->m_pNextItem;
            else
                m_pHashedItems[index] = pItem->m_pNextItem;

            pItemContent = pItem->m_pItem;
            if (m_iItemCount > 0)
                m_iItemCount--;

            // delete item, but not the contents, pass it back to be deleted.
            delete pItem;
            return pItemContent;
        }

        // next item
        pLast = pItem;
        pItem = pItem->m_pNextItem;
    }

    return 0;
}

template <class T>
u32 ListMap<T>::GetFreeID(u32 max) const
{
    u32 iID = m_iLastID;
    u32 start = iID;

    iID++;
    if (iID > max)
    {
        iID = 1;
        start = max;
    }

    while (GetItem(iID) && iID != start)
    {
        iID++;
        if (iID > max)
            iID = 1;
    }

    if (GetItem(iID))
        iID = 0;
    return iID;
}

template <class T>
void ListMap<T>::ClearAll()
{
    if (m_iItemCount > 0)
    {
        for (u32 i = 0; i < m_iListSize; i++)
        {
            while (m_pHashedItems[i])
            {
                HashedItem *pItem = m_pHashedItems[i];
                m_pHashedItems[i] = m_pHashedItems[i]->m_pNextItem;

                // don't delete item contents, only the item itself
                delete pItem;
            }
        }
    }

    m_iLastID = 100000;
    m_iItemCount = 0;
    m_pIter = 0;
    m_iIterIndex = 0;
}

// String versions

template <class T>
void ListMap<T>::AddItem(T pItem, const char *szID)
{
    if (!szID)
        return;
    // if ( GetItem( szID ) ) return;
    unsigned int iLength = 0;
    u32 index = HashIndexStr(szID, &iLength);

    HashedItem *pNewItem = new HashedItem();
    pNewItem->m_iItemStrLength = iLength;
    pNewItem->m_strIndex = new char[iLength + 1];
    memcpy(pNewItem->m_strIndex, szID, iLength + 1);
    pNewItem->m_pItem = pItem;

    pNewItem->m_pNextItem = m_pHashedItems[index];
    m_pHashedItems[index] = pNewItem;

    m_iItemCount++;
}

template <class T>
T ListMap<T>::GetItem(const char *szID) const
{
    if (m_iItemCount == 0)
        return 0;

    if (!szID)
        return 0;
    unsigned int iLength = 0;
    u32 index = HashIndexStr(szID, &iLength);
    HashedItem *pItem = m_pHashedItems[index];
    while (pItem)
    {
        if (pItem->m_iItemStrLength == iLength && pItem->m_strIndex && strcmp(szID, pItem->m_strIndex) == 0)
            return pItem->m_pItem;
        pItem = pItem->m_pNextItem;
    }

    return 0;
}

template <class T>
T ListMap<T>::RemoveItem(const char *szID)
{
    unsigned int iLength = 0;
    u32 index = HashIndexStr(szID, &iLength);
    HashedItem *pItem = m_pHashedItems[index];
    HashedItem *pLast = 0;
    T pItemContent = 0;

    while (pItem)
    {
        // check if this item matches target
        if (pItem->m_iItemStrLength == iLength && pItem->m_strIndex && strcmp(szID, pItem->m_strIndex) == 0)
        {
            // if this item is the current iterator then re-calculate the next step
            if (pItem == m_pIter)
                GetNext();

            // remove the item from the list
            if (pLast)
                pLast->m_pNextItem = pItem->m_pNextItem;
            else
                m_pHashedItems[index] = pItem->m_pNextItem;

            pItemContent = pItem->m_pItem;
            m_iItemCount--;

            // delete item, but not the contents, pass it back to be deleted.
            delete pItem;
            return pItemContent;
        }

        // next item
        pLast = pItem;
        pItem = pItem->m_pNextItem;
    }

    return 0;
}

// Pointer versions

template <class T>
void ListMap<T>::AddItem(T pItem, const void *ptr)
{
    // if ( GetItem( ptr ) ) return;
    u32 index = HashIndexPtr(ptr);

    HashedItem *pNewItem = new HashedItem();
    pNewItem->m_iItemStrLength = 0;
    pNewItem->m_ptrIndex = ptr;
    pNewItem->m_pItem = pItem;

    pNewItem->m_pNextItem = m_pHashedItems[index];
    m_pHashedItems[index] = pNewItem;

    m_iItemCount++;
}

template <class T>
T ListMap<T>::GetItem(const void *ptr) const
{
    if (m_iItemCount == 0)
        return 0;

    u32 index = HashIndexPtr(ptr);
    HashedItem *pItem = m_pHashedItems[index];
    while (pItem)
    {
        if (pItem->m_ptrIndex == ptr)
            return pItem->m_pItem;
        pItem = pItem->m_pNextItem;
    }

    return 0;
}

template <class T>
T ListMap<T>::RemoveItem(const void *ptr)
{
    u32 index = HashIndexPtr(ptr);
    HashedItem *pItem = m_pHashedItems[index];
    HashedItem *pLast = 0;
    T pItemContent = 0;

    while (pItem)
    {
        // check if this item matches target
        if (pItem->m_ptrIndex == ptr)
        {
            // if this item is the current iterator then re-calculate the next step in advance
            if (pItem == m_pIter)
                GetNext();

            // remove the item from the list
            if (pLast)
                pLast->m_pNextItem = pItem->m_pNextItem;
            else
                m_pHashedItems[index] = pItem->m_pNextItem;

            pItemContent = pItem->m_pItem;
            m_iItemCount--;

            // delete item, but not the contents as it is undefined, pass it back to be deleted.
            delete pItem;
            return pItemContent;
        }

        // next item
        pLast = pItem;
        pItem = pItem->m_pNextItem;
    }

    return 0;
}

