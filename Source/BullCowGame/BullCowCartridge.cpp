// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() //When the game starts
{
    Super::BeginPlay();

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("Lists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [this](const FString& Word) { return IsIsogram(Word); });

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (!bGameOver)
    {
        ProcessGuess(PlayerInput); //else Checking Player guess
    }
    else
    {
        ClearScreen();
        SetupGame();
    }
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len() + HiddenWord.Len() / 2;
    bGameOver = false;

    //Welcoming Players
    PrintLine(TEXT("Welcome to Bull Cows"));
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives!"), Lives);
    PrintLine(TEXT("Type in your guess and, \npress ENTER to continue...")); //Prompt Player for guess;
    //PrintLine(FString::Printf(TEXT("The HiddenWord is: %s. \nIt is %i characters long."), *HiddenWord, HiddenWord.Len())); //Debug line
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    //Check if Lives > 0
    if (Lives <= 1)
    {
        //If no show GameOver and HiddenWord?
        ClearScreen();
        PrintLine(TEXT("Sadly you lost!"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame(); //Prompt to Play Again, Press Enter to Play Again
        return;
    }

    if (Guess.Equals(HiddenWord))
    {
        ClearScreen();
        PrintLine(TEXT("You have Won!"));
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        ClearScreen();
        //Show Lives left
        PrintLine(TEXT("Try guessing again! \nYou have %i lives remaining."), --Lives); //Remove Life
        PrintLine(TEXT("The hidden word is %i letters long."), HiddenWord.Len());
        return;
    }

    // Check if Isogram
    if (!IsIsogram(Guess))
    {
        ClearScreen();
        PrintLine(TEXT("No repeating letters, guess again!"));
        return;
    }

    //remove a life
    ClearScreen();
    PrintLine(TEXT("Try guessing again!"));
    --Lives;

    //Show the player Bulls and Cows
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

    PrintLine(TEXT("You have %i lives left!"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
                return false;
        }
    }
    if(Word.Len() >= 4 && Word.Len() <= 8)
        return true;

    return false;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}
