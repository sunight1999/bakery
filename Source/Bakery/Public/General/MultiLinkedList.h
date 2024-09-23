// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

template<typename ValueType>
struct BAKERY_API FMultiLinkedListNode
{
public:
	FMultiLinkedListNode();

private:
	TArray<FMultiLinkedListNode*> PrevNodes;
	ValueType* Value;
	TArray<FMultiLinkedListNode*> NextNodes;
};

/**
 * 
 */
template<typename IdType, typename ValueType>
class BAKERY_API TMultiLinkedList
{
public:
	TMultiLinkedList();
	~TMultiLinkedList();

	const FMultiLinkedListNode<ValueType>* Find(IdType Id) const;
	void Add(IdType Id, ValueType* Value, TArray<IdType> PrevNodeIds);
	void Insert(IdType Id, ValueType* Value, IdType PrevNodeId, IdType NextNodeId);
	void Remove(IdType Id);

private:
	FMultiLinkedListNode<ValueType>* RootNode;
	TMap<IdType, FMultiLinkedListNode<ValueType>*> IndexMap;
};

