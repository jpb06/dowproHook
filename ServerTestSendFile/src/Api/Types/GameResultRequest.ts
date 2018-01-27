import { GameResult, isGameResult } from './GameResult';

export interface GameResultRequest {
    Identity: string;
    GameResult: GameResult;
}

export function isGameResultRequest(obj: any): obj is GameResultRequest {
    return (
        typeof obj.Identity === "string" &&
        isGameResult(obj.GameResult)
    );
}