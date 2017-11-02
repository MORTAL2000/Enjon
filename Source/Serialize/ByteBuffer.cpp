// @file ByteBuffer.cpp
// Copyright 2016-2017 John Jackson. All Rights Reserved.
 
#include "Serialize/ByteBuffer.h"
#include "Utils/FileUtils.h"
#include "Serialize/UUID.h"

#include <stdlib.h>
#include <assert.h>
#include <fstream>

#define BYTE_BUFFER_RW( type)\
	template type ByteBuffer::Read< type >( );\
	template void ByteBuffer::Write< type >( const type& val );

namespace Enjon
{ 
	//========================================================================

	ByteBuffer::ByteBuffer( )
	{ 
		// Allocate memory for buffer
		mBuffer = ( u8* )malloc( sizeof( u8 ) * mCapacity );
		assert( mBuffer != nullptr );
	}

	//========================================================================
	
	ByteBuffer::~ByteBuffer( )
	{ 
		ReleaseData( );
	}

	//========================================================================

	void ByteBuffer::ReleaseData( )
	{
		// Delete all of its data
		delete mBuffer;
		mBuffer = nullptr; 
	}

	//========================================================================

	void ByteBuffer::Reset( )
	{
		ReleaseData( );

		// Reset to default values
		mCapacity = 1024;
		mSize = 0;
		mReadPosition	= 0;
		mWritePosition	= 0;

		// Reallocate memory for buffer
		mBuffer = ( u8* )malloc( sizeof( u8 ) * mCapacity );
		assert( mBuffer != nullptr );
	}

	//========================================================================
			
	void ByteBuffer::Resize( usize size )
	{
		mBuffer = ( u8* )realloc( mBuffer, sizeof( u8 ) * size );
		mReadPosition = 0;
		assert( mBuffer != nullptr );
	}

	//========================================================================

	template <typename T>
	T ByteBuffer::Read( )
	{ 
		// Get size of T
		usize size = sizeof( T ); 

		// Get value at position
		T val = *( T* )( mBuffer + mReadPosition );

		// Increment read position
		mReadPosition += size;

		return val;
	} 

	//========================================================================

	template <>
	String ByteBuffer::Read< String >( )
	{
		// Get size of T from buffer
		usize size = Read< usize >( );

		// String to fill out
		String val = "";

		// Get characters for string
		for ( usize i = 0; i < size; ++i ) 
		{
			// Get character
			char c = *( char* )( mBuffer + mReadPosition );

			// Append character to string
			val.push_back( c );
			
			// Increment read position
			mReadPosition += 1;
		} 

		return val;
	}

	//========================================================================

	template <>
	UUID ByteBuffer::Read< UUID >( )
	{
		// Get hash string for uuid
		String uuidHash = Read< String >( );

		// Construct new uuid
		UUID uuid( uuidHash );

		return uuid;
	}

	//========================================================================

	template < typename T >
	void ByteBuffer::Write( const T& val )
	{ 
		// Get size of val
		usize size = sizeof( T ); 

		// Make sure that enough bytes are present in buffer
		if ( mWritePosition + size >= mCapacity )
		{
			// Resize the buffer by twice the original capacity
			mCapacity *= 2;
			Resize( mCapacity );
		}

		// Now write to the buffer
		*( T* )( mBuffer + mWritePosition ) = val;

		// Increment position by size of val
		mWritePosition += size;
		mSize += size;
	}

	template<>
	void ByteBuffer::Write< String >( const String& val )
	{
		// Get size of val
		usize size = val.length( );

		if ( mWritePosition + size + sizeof( usize ) >= mCapacity )
		{
			mCapacity *= 2;
			Resize( mCapacity );
		}

		// Write length of string
		Write< usize >( size );

		// Write characters of string
		for ( auto& c : val ) 
		{ 
			// Write to buffer
			*( char* )( mBuffer + mWritePosition ) = c;

			// Increment by 1
			mWritePosition += 1;
			mSize += 1;
		} 
	}

	template<>
	void ByteBuffer::Write< UUID >( const UUID& val )
	{
		// Get hashed string of uuid
		String uuidHash = val.ToString( );

		// Write to buffer
		Write< String >( uuidHash );
	}

	//========================================================================
			
	void ByteBuffer::ReadFromFile( const Enjon::String& filePath )
	{ 
		std::ifstream infile;
		infile.open( filePath.c_str( ), std::ios::in | std::ios::binary );

		if ( infile )
		{
			u8* oData = 0;

			// Get file size
			infile.seekg( 0, std::ios::end );
			usize size = infile.tellg( ); 
			infile.seekg( 0, std::ios::beg );

			// Create and read data
			oData = new u8[ size + 1 ];
			infile.read( (char*)oData, size );
			oData[ size ] = '\0';

			// Delete previous buffer that was allocated
			if ( mBuffer )
			{
				delete mBuffer;
			} 

			// Set buffer to oData and reset fields
			mBuffer = oData;
			mSize = size;
			mReadPosition = 0;
			mWritePosition = 0;
		} 
	}

	//========================================================================

	void ByteBuffer::WriteToFile( const Enjon::String& filePath )
	{ 
		// Open file 
		std::ofstream file( filePath, std::ios::out | std::ios::binary ); 

		// Only write if file is valid
		if ( file )
		{
			file.write( (char*)mBuffer, mSize );
		} 
	}

	//========================================================================

	const u32 ByteBuffer::GetSize( ) const
	{
		return mSize;
	}

	//========================================================================

	// Template Specializations
	BYTE_BUFFER_RW( b1 )
	BYTE_BUFFER_RW( s8 )
	BYTE_BUFFER_RW( u8 )
	BYTE_BUFFER_RW( char )
	BYTE_BUFFER_RW( u16 )
	BYTE_BUFFER_RW( s16 )
	BYTE_BUFFER_RW( s32 )
	BYTE_BUFFER_RW( u32 )
	BYTE_BUFFER_RW( f32 )
	BYTE_BUFFER_RW( s64 )
	BYTE_BUFFER_RW( u64 )
	BYTE_BUFFER_RW( f64 )
	BYTE_BUFFER_RW( usize )
}
