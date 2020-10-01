#include "action.h"
#include "twiddle.h"
#include "eval.h"
#include "hash.h"
#include <cstdint>



board doMove(board startBoard, move_t move) {
    int i;
    uint16_t fromSquare = move.from_sq();
    uint16_t toSquare   = move.to_sq();
    bool castling[4];
    bool ep;
    startBoard.getEP(&ep);
    int dpp;
    startBoard.getdPPFile(&dpp);
    int clk;
    int full_clk;
    uint64_t hash;
    startBoard.getHash( &hash );
    int32_t value = startBoard.getValue();
    colour movingColour;
    startBoard.getSide( & movingColour );
    //  int plus_minus = ( movingColour == white ) ? 1 : -1;
    colour otherColour = ( movingColour == white ) ? black : white;
    colourPiece movingPiece;
    bool rooktaken=false;

    bitboard startingPos[12];

    startBoard.getBitboards(startingPos);

    for (i=(movingColour*6);i<(1+movingColour)*6;i++) {
        if ( is_bit_set(startingPos[i], fromSquare) ) {
            movingPiece = colourPiece(i);
            startingPos[i] = (startingPos[i] & ~( 1ULL << fromSquare) ) | ( 1ULL << toSquare );
            value -= pieceSquareTables[i][fromSquare];
            value += pieceSquareTables[i][toSquare];
            hash ^= zobristKeys[i*64 + fromSquare];
            hash ^= zobristKeys[i*64 + toSquare];
            break;
            }
        }


    if ( move.is_capture() ) {
        if ( ! move.is_ep_capture() ) {
            for (i=(1-movingColour)*6;i<(2-movingColour)*6;i++) {
                if (is_bit_set(startingPos[i], toSquare) ) {
                    if ( i%6 == 1 ) rooktaken = true;
                    startingPos[i] = (startingPos[i] & ~( 1ULL << toSquare) );
                    value -= pieceSquareTables[i][toSquare];
                    value -= pieceValues[i];
                    hash ^= zobristKeys[i*64 + toSquare];
                    break;
                    }
                }
            }

        else {
            int _dir = ( movingColour == white ) ? S : N;
            startingPos[(1-movingColour)*6] &= ~( 1ULL << (toSquare + _dir) );
            value -= pieceSquareTables[(1-movingColour)*6][toSquare + _dir];
            value -= pieceValues[(1-movingColour)*6];
            hash ^= zobristKeys[(1-movingColour)*6*64 + toSquare + _dir];
            }
        }

    if ( move.is_kingCastle() ) {
        startingPos[1+(6*movingColour)] = (startingPos[1+(6*movingColour)] & ~( 1ULL << (fromSquare+3)) )
        | ( 1ULL << (toSquare-1) );
        value -= pieceSquareTables[1+(6*movingColour)][fromSquare+3];
        value += pieceSquareTables[1+(6*movingColour)][toSquare-1];
        hash ^= zobristKeys[(1+(6*movingColour))*64 + fromSquare + 3];
        hash ^= zobristKeys[(1+(6*movingColour))*64 + toSquare - 1];
        }
    else if ( move.is_queenCastle() ) {
        startingPos[1+(6*movingColour)] = (startingPos[1+(6*movingColour)] & ~( 1ULL << (fromSquare-4)) )
        | ( 1ULL << (toSquare+1) );
        value -= pieceSquareTables[1+(6*movingColour)][fromSquare-4];
        value += pieceSquareTables[1+(6*movingColour)][toSquare+1];
        hash ^= zobristKeys[(1+(6*movingColour))*64 + fromSquare - 4];
        hash ^= zobristKeys[(1+(6*movingColour))*64 + toSquare + 1];
        }

    // promotion
    if ( move.is_promotion() ) {
        startingPos[6*movingColour] &= ( ~ ( 1ULL << toSquare ) );
        value -= pieceSquareTables[6*movingColour][toSquare];
        value -= pieceValues[6*movingColour];
        //    std::cout << "--- added " << - int(pieceSquareTables[(6*movingColour)][toSquare]);
        //    std::cout << " to value ---\n";
        hash ^= zobristKeys[6*movingColour*64 + toSquare];

        colourPiece prom_piece = colourPiece( (6*movingColour) + move.which_promotion() );

        startingPos[prom_piece] |= ( 1ULL << toSquare );
        value += pieceSquareTables[prom_piece][toSquare];
        value += pieceValues[prom_piece];
        //            std::cout << "--- added " << int(pieceSquareTables[prom_piece][toSquare]);
        //            std::cout << " to value ---\n";
        hash ^= zobristKeys[prom_piece*64 + toSquare];
        }

    startBoard.getCastlingRights(castling);
    startBoard.getClock(&clk);
    startBoard.getFullClock(&full_clk);

    // check for double pawn push
    if ( ep ) {
        hash ^= zobristKeys[772 + dpp];
        }
    if (move.is_doublePP()) {
        ep=true;
        dpp= fromSquare%8;
        hash ^= zobristKeys[772 + dpp];
        }
    else ep=false;

    // check for changes to castling rights
    if (movingPiece % 6 == 1) {
        switch (fromSquare) {
            case 0:
            if (castling[1]) {
                hash ^= zobristKeys[769];
                castling[1] = false;
                }
            break;
            case 7:
            if (castling[0]) {
                hash ^= zobristKeys[768];
                castling[0] = false;
                }
            break;
            case 56:
            if (castling[3]) {
                hash ^= zobristKeys[771];
                castling[3] = false;
                }
            break;
            case 63:
            if (castling[2]) {
                hash ^= zobristKeys[770];
                castling[2] = false;
                }
            break;
            }
        }
    else if (movingPiece % 6 == 5) {
        switch (movingColour) {
            case white:
            if (castling[1]) {
                hash ^= zobristKeys[769];
                castling[1] = false;
                }
            if (castling[0]) {
                hash ^= zobristKeys[768];
                castling[0] = false;
                }
            break;
            case black:
            if (castling[3]) {
                hash ^= zobristKeys[771];
                castling[3] = false;
                }
            if (castling[2]) {
                hash ^= zobristKeys[770];
                castling[2] = false;
                }
            break;
            }
        }

    if ( rooktaken ) {
        switch ( toSquare ) {
            case 0:
            if (castling[1]) {
                hash ^= zobristKeys[769];
                castling[1] = false;
                }
            break;
            case 7:
            if (castling[0]) {
                hash ^= zobristKeys[768];
                castling[0] = false;
                }
            break;
            case 56:
            if (castling[3]) {
                hash ^= zobristKeys[771];
                castling[3] = false;
                }
            break;
            case 63:
            if (castling[2]) {
                hash ^= zobristKeys[770];
                castling[2] = false;
                }
            break;
            }
        }

    // increment halfMoveClock
    if ( move.is_capture() | ( movingPiece % 6 == 0 ) ) clk = 0;
    else clk ++;

    // increment fullMoveClock
    if ( movingColour == black ) full_clk++;

    // change hash for different side to move
    hash ^= zobristKeys[780];

    board endBoard (startingPos, castling, ep, dpp, clk, full_clk, otherColour, value, hash );

    return endBoard;
    }



void board::doMoveInPlace( move_t move ) {
    int i;
    uint16_t fromSquare = move.from_sq();
    uint16_t toSquare   = move.to_sq();
    //  int plus_minus = ( sideToMove == white ) ? 1 : -1;
    colour otherColour = ( sideToMove == white ) ? black : white;
    colourPiece movingPiece;
    bool rooktaken=false;

    for (i=(sideToMove*6);i<(1+sideToMove)*6;i++) {
        if ( is_bit_set(pieceBoards[i], fromSquare) ) {
            movingPiece = colourPiece(i);
            pieceBoards[i] = (pieceBoards[i] & ~( 1ULL << fromSquare) ) | ( 1ULL << toSquare );
            value -= pieceSquareTables[i][fromSquare];
            value += pieceSquareTables[i][toSquare];
            hash_value ^= zobristKeys[i*64 + fromSquare];
            hash_value ^= zobristKeys[i*64 + toSquare];
            break;
            }
        }


    if ( move.is_capture() ) {
        if ( ! move.is_ep_capture() ) {
            for (i=(1-sideToMove)*6;i<(2-sideToMove)*6;i++) {
                if (is_bit_set(pieceBoards[i], toSquare) ) {
                    if ( i%6 == 1 ) rooktaken = true;
                    pieceBoards[i] = (pieceBoards[i] & ~( 1ULL << toSquare) );
                    value -= pieceSquareTables[i][toSquare];
                    value -= pieceValues[i];
                    hash_value ^= zobristKeys[i*64 + toSquare];
                    break;
                    }
                }
            }

        else {
            int _dir = ( sideToMove == white ) ? S : N;
            pieceBoards[(1-sideToMove)*6] &= ~( 1ULL << (toSquare + _dir) );
            value -= pieceSquareTables[(1-sideToMove)*6][toSquare + _dir];
            value -= pieceValues[(1-sideToMove)*6];
            hash_value ^= zobristKeys[(1-sideToMove)*6*64 + toSquare + _dir];
            }
        }

    if ( move.is_kingCastle() ) {
        pieceBoards[1+(6*sideToMove)] = (pieceBoards[1+(6*sideToMove)] & ~( 1ULL << (fromSquare+3)) )
        | ( 1ULL << (toSquare-1) );
        value -= pieceSquareTables[1+(6*sideToMove)][fromSquare+3];
        value += pieceSquareTables[1+(6*sideToMove)][toSquare-1];
        hash_value ^= zobristKeys[(1+(6*sideToMove))*64 + fromSquare + 3];
        hash_value ^= zobristKeys[(1+(6*sideToMove))*64 + toSquare - 1];
        }
    else if ( move.is_queenCastle() ) {
        pieceBoards[1+(6*sideToMove)] = (pieceBoards[1+(6*sideToMove)] & ~( 1ULL << (fromSquare-4)) )
        | ( 1ULL << (toSquare+1) );
        value -= pieceSquareTables[1+(6*sideToMove)][fromSquare-4];
        value += pieceSquareTables[1+(6*sideToMove)][toSquare+1];
        hash_value ^= zobristKeys[(1+(6*sideToMove))*64 + fromSquare - 4];
        hash_value ^= zobristKeys[(1+(6*sideToMove))*64 + toSquare + 1];
        }

    // promotion
    if ( move.is_promotion() ) {
        pieceBoards[6*sideToMove] &= ( ~ ( 1ULL << toSquare ) );
        value -= pieceSquareTables[6*sideToMove][toSquare];
        value -= pieceValues[6*sideToMove];
        //    std::cout << "--- added " << - int(pieceSquareTables[(6*sideToMove)][toSquare]);
        //    std::cout << " to value ---\n";
        hash_value ^= zobristKeys[6*sideToMove*64 + toSquare];

        colourPiece prom_piece = colourPiece( (6*sideToMove) + move.which_promotion() );

        pieceBoards[prom_piece] |= ( 1ULL << toSquare );
        value += pieceSquareTables[prom_piece][toSquare];
        value += pieceValues[prom_piece];
        //            std::cout << "--- added " << int(pieceSquareTables[prom_piece][toSquare]);
        //            std::cout << " to value ---\n";
        hash_value ^= zobristKeys[prom_piece*64 + toSquare];
        }


    // check for double pawn push
    if ( lastMoveDoublePawnPush ) {
        hash_value ^= zobristKeys[772 + dPPFile];
        }
    if (move.is_doublePP()) {
        lastMoveDoublePawnPush=true;
        dPPFile= fromSquare%8;
        hash_value ^= zobristKeys[772 + dPPFile];
        }
    else lastMoveDoublePawnPush=false;

    // check for changes to castling rights
    if (movingPiece % 6 == 1) {
        switch (fromSquare) {
            case 0:
            if (castleWhiteQueenSide) {
                hash_value ^= zobristKeys[769];
                castleWhiteQueenSide = false;
                }
            break;
            case 7:
            if (castleWhiteKingSide) {
                hash_value ^= zobristKeys[768];
                castleWhiteKingSide = false;
                }
            break;
            case 56:
            if (castleBlackQueenSide) {
                hash_value ^= zobristKeys[771];
                castleBlackQueenSide = false;
                }
            break;
            case 63:
            if (castleBlackKingSide) {
                hash_value ^= zobristKeys[770];
                castleBlackKingSide = false;
                }
            break;
            }
        }
    else if (movingPiece % 6 == 5) {
        switch (sideToMove) {
            case white:
            if (castleWhiteQueenSide) {
                hash_value ^= zobristKeys[769];
                castleWhiteQueenSide = false;
                }
            if (castleWhiteKingSide) {
                hash_value ^= zobristKeys[768];
                castleWhiteKingSide = false;
                }
            break;
            case black:
            if (castleBlackQueenSide) {
                hash_value ^= zobristKeys[771];
                castleBlackQueenSide = false;
                }
            if (castleBlackKingSide) {
                hash_value ^= zobristKeys[770];
                castleBlackKingSide = false;
                }
            break;
            }
        }

    if ( rooktaken ) {
        switch ( toSquare ) {
            case 0:
            if (castleWhiteQueenSide) {
                hash_value ^= zobristKeys[769];
                castleWhiteQueenSide = false;
                }
            break;
            case 7:
            if (castleWhiteKingSide) {
                hash_value ^= zobristKeys[768];
                castleWhiteKingSide = false;
                }
            break;
            case 56:
            if (castleBlackQueenSide) {
                hash_value ^= zobristKeys[771];
                castleBlackQueenSide = false;
                }
            break;
            case 63:
            if (castleBlackKingSide) {
                hash_value ^= zobristKeys[770];
                castleBlackKingSide = false;
                }
            break;
            }
        }

    // increment halfMoveClock
    if ( move.is_capture() | ( movingPiece % 6 == 0 ) ) halfMoveClock = 0;
    else halfMoveClock ++;

    // increment fullMoveClock
    if ( sideToMove == black ) fullMoveClock++;

    // change hash for different side to move
    hash_value ^= zobristKeys[780];

    }

