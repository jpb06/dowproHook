import { GameResultRequest, isGameResultRequest } from './Types/GameResultRequest';

export async function Handle(data: Buffer): Promise<boolean> {
    let raw = data.toString('utf8');

    let json: string;
    try {
        json = JSON.parse(raw);
    } catch(e) {
        console.log(e);
        return false;
    }

    if (!isGameResultRequest(json)) return false;
    let gameResultRequest = json as GameResultRequest;

    return true;
}