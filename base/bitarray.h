#pragma once
#include <cstdint>
#include <string>
#include <vector>
// Ĭ���������ֽ��ڲ��Ǵ��ֵ��󣬵����з�block��ʱ�򣬲��������ֽ�Ϊ��λ��
class bitarray
{
public:
    class reference
    {	// proxy for an element
        friend class bitarray;

    public:
        ~reference()
        {	// destroy the object
        }

        reference& operator=( bool _Val )
        {	// assign Boolean to element
            _Pbitset->set( _Mypos, _Val );
            return ( *this );
        }

            reference& operator=( const reference& _Bitref )
        {	// assign reference to element
            _Pbitset->set( _Mypos, bool( _Bitref ) );
            return ( *this );
        }

            reference& flip()
        {	// complement stored element
            _Pbitset->flip( _Mypos );
            return ( *this );
        }

            bool operator~( ) const
        {	// return complemented element
            return ( !_Pbitset->test( _Mypos ) );
        }

            operator bool() const
        {	// return element
            return ( _Pbitset->test( _Mypos ) );
        }

    private:
        reference()
            : _Pbitset( 0 ), _Mypos( 0 )
        {	// default construct
        }

        reference( bitarray& _Bitset, size_t _Pos )
            : _Pbitset( &_Bitset ), _Mypos( _Pos )
        {	// construct from bitset reference and position
        }

        bitarray *_Pbitset;	// pointer to the bitset
        size_t _Mypos;	// position of element in bitset
    };

    bitarray( size_t N );
    ~bitarray();
    size_t bits()const;
    void set( size_t pos, uint32_t value, int nbit);
    void set( size_t pos, bool value = true );
    bool set(int pos, std::vector<std::pair< int, int>> v );//start ��λ���Ļ�������һ����int ��value,�ڶ�����ȥ��͵���Чλ������0��ʼ�ۼ�
    bool get( int pos, std::vector<std::pair<int, int>>&blocks );
    bool test( size_t pos )const;
    reference operator []( size_t pos );
    bool operator[]( size_t _Pos )const;
    bitarray &flip();
    bitarray&flip(size_t pos);
    std::string to_string();
    std::vector<uint32_t> to_uint32s();
    uint32_t              to_uint32(); // ��ˣ�������
    uint32_t              to_uint32l();//С��
    uint8_t*data();

private:
    uint8_t* m_array;
    size_t   m_nBits;
    size_t   m_bytes;
};