// SiasOthelloAI.cpp

#include "SiasOthelloAI.hpp"
#include "OthelloException.hpp"
#include <stack>
#include <chrono>



namespace
{
	int search(const OthelloGameState& state, unsigned int depth, const std::chrono::time_point<std::chrono::high_resolution_clock> startTime);
	int evaluateState(const OthelloGameState& state);
	std::vector<std::pair<int,int>> findValidMoves(const OthelloGameState& state);



	int search(const OthelloGameState& state, unsigned int depth, const std::chrono::time_point<std::chrono::high_resolution_clock> startTime)
	{
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime);

		int stateValue;

		if (--depth == 0 || state.isGameOver() || duration.count() > 2800)
		{
			stateValue = evaluateState(state);
		}

		else if (state.isBlackTurn())
		{
			stateValue = -999;
			for (auto move : findValidMoves(state))
			{
				std::unique_ptr<OthelloGameState> newState (state.clone());
				newState->makeMove(move.first, move.second);
				int moveValue = search(*newState, depth, startTime);
				if (moveValue >= stateValue)
				{
					stateValue = moveValue;
				}
			}
		}

		else // it is White's turn
		{
			stateValue = 999;
			for (auto move : findValidMoves(state))
			{
				std::unique_ptr<OthelloGameState> newState (state.clone());
				newState->makeMove(move.first, move.second);
				int moveValue = search(*newState, depth, startTime);
				if (moveValue <= stateValue)
					stateValue = moveValue;
			}
		}

		return stateValue;
	}



	int evaluateState(const OthelloGameState& state)
	{
		int scoreDifferential = state.blackScore() - state.whiteScore();

		if (state.isGameOver())
		{
			if (state.blackScore() > state.whiteScore())
				scoreDifferential += 4;

			else // white wins
				scoreDifferential -= 4;
		}

		// bonuses to black for holding corner squares
		if (state.board().cellAt(0,0) == OthelloCell::black)
		{
			scoreDifferential += 2;
		}

		if (state.board().cellAt(state.board().width()-1,0) == OthelloCell::black)
		{
			scoreDifferential += 2;
		}

		if (state.board().cellAt(0,state.board().height()-1) == OthelloCell::black)
		{
			scoreDifferential += 2;
		}

		if (state.board().cellAt(state.board().width()-1, state.board().height()-1) == OthelloCell::black)
		{
			scoreDifferential += 2;
		}


		// bonuses to white for holding corner squares
		if (state.board().cellAt(0,0) == OthelloCell::white)
		{
			scoreDifferential -= 2;
		}

		if (state.board().cellAt(state.board().width()-1,0) == OthelloCell::white)
		{
			scoreDifferential -= 2;
		}

		if (state.board().cellAt(0,state.board().height()-1) == OthelloCell::white)
		{
			scoreDifferential -= 2;
		}

		if (state.board().cellAt(state.board().width()-1, state.board().height()-1) == OthelloCell::white)
		{
			scoreDifferential -= 2;
		}


		return scoreDifferential;
	}



	std::vector<std::pair<int,int>> findValidMoves(const OthelloGameState& state)
	{
		std::vector<std::pair<int,int>> validMoves;

		for (int x = 0; x < state.board().width(); ++x)
			for (int y = 0; y < state.board().height(); ++y)
			{
				if (state.isValidMove(x,y))
					validMoves.push_back(std::pair<int,int>{x,y});
			}
		return validMoves;
	}

}



// PUBLIC

std::pair<int, int> siash::SiasOthelloAI::chooseMove(const OthelloGameState& state)
{
	unsigned int searchDepth = 5;
	int stateValue;
	std::pair<int,int> moveChoice;

	const std::chrono::time_point<std::chrono::high_resolution_clock> startTime = std::chrono::high_resolution_clock::now();

	if (state.isBlackTurn())
	{
		stateValue = -999;
		for (auto move : findValidMoves(state))
		{
			std::unique_ptr<OthelloGameState> newState (state.clone());
			newState->makeMove(move.first, move.second);
			int moveValue = search(*newState, searchDepth, startTime);
			if (moveValue >= stateValue)
			{
				stateValue = moveValue;
				moveChoice = std::pair<int,int>{move.first,move.second};
			}
		}
	}

	else // it is White's turn
	{
		stateValue = 999;
		for (auto move : findValidMoves(state))
		{
			std::unique_ptr<OthelloGameState> newState (state.clone());
			newState->makeMove(move.first, move.second);
			int moveValue = search(*newState, searchDepth, startTime);

			if (moveValue <= stateValue)
			{
				stateValue = moveValue;
				moveChoice = std::pair<int,int>{move.first,move.second};
			}
		}
	}

	return moveChoice;
}
