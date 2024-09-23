// Fill out your copyright notice in the Description page of Project Settings.


#include "General/MultiLinkedList.h"

/*
 * MultiLinkedList Struct Definitions
 */
template<typename ValueType>
inline FMultiLinkedListNode<ValueType>::FMultiLinkedListNode()
{
	
}


/*
 * MultiLinkedList Class Definitions
 */
template<typename IdType, typename ValueType>
TMultiLinkedList<IdType, ValueType>::TMultiLinkedList()
{
	RootNode = new ValueType();
}

template<typename IdType, typename ValueType>
TMultiLinkedList<IdType, ValueType>::~TMultiLinkedList()
{
}

template<typename IdType, typename ValueType>
const FMultiLinkedListNode<ValueType>* TMultiLinkedList<IdType, ValueType>::Find(IdType Id) const
{
	return IndexMap.Find(Id);
}

template<typename IdType, typename ValueType>
void TMultiLinkedList<IdType, ValueType>::Add(IdType Id, ValueType* Value, TArray<IdType> PrevNodeIds)
{
}

template<typename IdType, typename ValueType>
void TMultiLinkedList<IdType, ValueType>::Insert(IdType Id, ValueType* Value, IdType PrevNodeId, IdType NextNodeId)
{
}

template<typename IdType, typename ValueType>
void TMultiLinkedList<IdType, ValueType>::Remove(IdType Id)
{
}
